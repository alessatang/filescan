#include <pcre.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utime.h"
#include "unistd.h"
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "config.h"

//#ifdef HAVE_PTHREAD_H
#include <pthread.h>
//#endif

#include "log.h"
#include "options.h"
#include "search.h"
#include "util.h"

typedef struct {
    pthread_t thread;
    int id;
} worker_t;

int wmain(int argc, wchar_t **argv) {
	wchar_t **base_paths = NULL;
	wchar_t **paths = NULL;
    int i;
    int pcre_opts = PCRE_MULTILINE;
    int study_opts = 0;
    double time_diff;
    worker_t *workers = NULL;
    int workers_len;
    int num_cores;

#ifdef KJK_BUILD
    extern void setup_crash_handler(); /* in kjk_crash_handler.cpp */
    setup_crash_handler();
#endif

    set_log_level(LOG_LEVEL_WARN);

    work_queue = NULL;
    work_queue_tail = NULL;
    memset(&stats, 0, sizeof(stats));
    root_ignores = init_ignore(NULL, L"", 0);
    out_fd = stdout;
#ifdef USE_PCRE_JIT
    int has_jit = 0;
    pcre_config(PCRE_CONFIG_JIT, &has_jit);
    if (has_jit) {
        study_opts |= PCRE_STUDY_JIT_COMPILE;
    }
#endif

    gettimeofday(&(stats.time_start), NULL);

    parse_options(argc, argv, &base_paths, &paths);
	log_debug(L"PCRE Version: %s", pcre16_version());
	setlocale(LC_ALL, "chs");

#ifdef _WIN32
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        num_cores = si.dwNumberOfProcessors;
    }
#else
    num_cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif

    workers_len = num_cores;
    if (opts.literal) {
        workers_len--;
    }
    if (opts.workers) {
        workers_len = opts.workers;
    }
    if (workers_len < 1) {
        workers_len = 1;
    }

    log_debug(L"Using %i workers", workers_len);
    done_adding_files = FALSE;
    workers = (worker_t *) ag_calloc(workers_len, sizeof(worker_t));
	if (pthread_cond_init(&files_ready, NULL)) {
        die(L"pthread_cond_init failed!");
    }
    if (pthread_mutex_init(&print_mtx, NULL)) {
        die(L"pthread_mutex_init failed!");
    }
    if (pthread_mutex_init(&stats_mtx, NULL)) {
        die(L"pthread_mutex_init failed!");
    }
    if (pthread_mutex_init(&work_queue_mtx, NULL)) {
        die(L"pthread_mutex_init failed!");
    }

    if (opts.casing == CASE_SMART) {
        opts.casing = is_lowercase(opts.query) ? CASE_INSENSITIVE : CASE_SENSITIVE;
    }

    if (opts.literal) {
        if (opts.casing == CASE_INSENSITIVE) {
            /* Search routine needs the query to be lowercase */
            wchar_t *c = opts.query;
            for (; *c != '\0'; ++c) {
				*c = (wchar_t)towlower(*c);
            }
        }
        generate_alpha_skip(opts.query, opts.query_len, alpha_skip_lookup, opts.casing == CASE_SENSITIVE);
        find_skip_lookup = NULL;
        generate_find_skip(opts.query, opts.query_len, &find_skip_lookup, opts.casing == CASE_SENSITIVE);
        if (opts.word_regexp) {
            init_wordchar_table();
            opts.literal_starts_wordchar = is_wordchar(opts.query[0]);
            opts.literal_ends_wordchar = is_wordchar(opts.query[opts.query_len - 1]);
        }
    } else {
        if (opts.casing == CASE_INSENSITIVE) {
            pcre_opts |= PCRE_CASELESS;
        }
        if (opts.word_regexp) {
            wchar_t *word_regexp_query;
            ag_asprintf(&word_regexp_query, L"\\b%s\\b", opts.query);
            free(opts.query);
            opts.query = word_regexp_query;
            opts.query_len = wcslen(opts.query);
        }
        compile_study(&opts.re, &opts.re_extra, opts.query, pcre_opts, study_opts);
    }

    if (opts.search_stream) {
        search_stream(stdin, L"");
    } else {
        for (i = 0; i < workers_len; i++) {
            workers[i].id = i;
            int rv = pthread_create(&(workers[i].thread), NULL, &search_file_worker, &(workers[i].id));
            if (rv != 0) {
                die(L"error in pthread_create(): %s", strerror(rv));
            }
#if defined(HAVE_PTHREAD_SETAFFINITY_NP) && defined(USE_CPU_SET)
            if (opts.use_thread_affinity) {
                cpu_set_t cpu_set;
                CPU_ZERO(&cpu_set);
                CPU_SET(i % num_cores, &cpu_set);
                rv = pthread_setaffinity_np(workers[i].thread, sizeof(cpu_set), &cpu_set);
                if (rv != 0) {
                    die("error in pthread_setaffinity_np(): %s", strerror(rv));
                }
                log_debug("Thread %i set to CPU %i", i, i);
            } else {
                log_debug("Thread affinity disabled.");
            }
#else
            log_debug(L"No CPU affinity support.");
#endif
        }
        for (i = 0; paths[i] != NULL; i++) {
            log_debug(L"searching path %s for %s", paths[i], opts.query);
            symhash = NULL;
            ignores *ig = init_ignore(root_ignores, L"", 0);
            struct stat s;
            s.st_dev = 0;
#ifndef _WIN32
            /* The device is ignored if opts.one_dev is false, so it's fine
             * to leave it at the default 0
             */
            if (opts.one_dev && lstat(paths[i], &s) == -1) {
                log_err("Failed to get device information for path %s. Skipping...", paths[i]);
            }
#endif
            search_dir(ig, base_paths[i], paths[i], 0, s.st_dev);
            cleanup_ignore(ig);
        }
        pthread_mutex_lock(&work_queue_mtx);
        done_adding_files = TRUE;
        pthread_cond_broadcast(&files_ready);
        pthread_mutex_unlock(&work_queue_mtx);
        for (i = 0; i < workers_len; i++) {
            if (pthread_join(workers[i].thread, NULL)) {
                die(L"pthread_join failed!");
            }
        }
    }

    if (opts.stats) {
        gettimeofday(&(stats.time_end), NULL);
        time_diff = ((long)stats.time_end.tv_sec * 1000000 + stats.time_end.tv_usec) -
                    ((long)stats.time_start.tv_sec * 1000000 + stats.time_start.tv_usec);
        time_diff /= 1000000;

        wprintf(L"%ld matches\n%ld files searched\n%ld bytes searched\n%f seconds\n", stats.total_matches, stats.total_files, stats.total_bytes, time_diff);
    }

    if (opts.pager) {
        pclose(out_fd);
    }
    cleanup_options();
    pthread_cond_destroy(&files_ready);
    pthread_mutex_destroy(&work_queue_mtx);
    pthread_mutex_destroy(&stats_mtx);
    pthread_mutex_destroy(&print_mtx);
    cleanup_ignore(root_ignores);
    free(workers);
    for (i = 0; paths[i] != NULL; i++) {
        free(paths[i]);
        free(base_paths[i]);
    }
    free(base_paths);
    free(paths);
    if (find_skip_lookup) {
        free(find_skip_lookup);
    }
    return !opts.match_found;
}