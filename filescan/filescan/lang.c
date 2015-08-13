#include <stdlib.h>
#include <string.h>

#include "lang.h"
#include "util.h"

lang_spec_t langs[] = {
    { L"actionscript", { L"as", L"mxml" } },
    { L"ada", { L"ada", L"adb", L"ads" } },
    { L"asm", { L"asm", L"s" } },
    { L"batch", { L"bat", L"cmd" } },
    { L"cc", { L"c", L"h", L"xs" } },
    { L"cfmx", { L"cfc", L"cfm", L"cfml" } },
    { L"clojure", { L"clj", L"cljs", L"cljc", L"cljx" } },
    { L"coffee", { L"coffee" } },
    { L"cpp", { L"cpp", L"cc", L"C", L"cxx", L"m", L"hpp", L"hh", L"h", L"H", L"hxx" } },
    { L"csharp", { L"cs" } },
    { L"css", { L"css" } },
    { L"delphi", { L"pas", L"int", L"dfm", L"nfm", L"dof", L"dpk", L"dproj", L"groupproj", L"bdsgroup", L"bdsproj" } },
    { L"ebuild", { L"ebuild", L"eclass" } },
    { L"elisp", { L"el" } },
    { L"erlang", { L"erl", L"hrl" } },
    { L"fortran", { L"f", L"f77", L"f90", L"f95", L"f03", L"for", L"ftn", L"fpp" } },
    { L"fsharp", { L"fs", L"fsi", L"fsx" } },
    { L"gettext", { L"po", L"pot", L"mo" } },
    { L"go", { L"go" } },
    { L"groovy", { L"groovy", L"gtmpl", L"gpp", L"grunit" } },
    { L"haml", { L"haml" } },
    { L"haskell", { L"hs", L"lhs" } },
    { L"hh", { L"h" } },
    { L"html", { L"htm", L"html", L"shtml", L"xhtml" } },
    { L"ini", { L"ini" } },
    { L"jade", { L"jade" } },
    { L"java", { L"java", L"properties" } },
    { L"js", { L"js", L"jsx" } },
    { L"json", { L"json" } },
    { L"jsp", { L"jsp", L"jspx", L"jhtm", L"jhtml" } },
    { L"less", { L"less" } },
    { L"liquid", { L"liquid" } },
    { L"lisp", { L"lisp", L"lsp" } },
    { L"lua", { L"lua" } },
    { L"m4", { L"m4" } },
    { L"make", { L"Makefiles", L"mk", L"mak" } },
    { L"markdown", { L"markdown", L"mdown", L"mdwn", L"mkdn", L"mkd", L"md" } },
    { L"mason", { L"mas", L"mhtml", L"mpl", L"mtxt" } },
    { L"matlab", { L"m" } },
    { L"mathematica", { L"m", L"wl" } },
    { L"mercury", { L"m", L"moo" } },
    { L"nim", { L"nim" } },
    { L"objc", { L"m", L"h" } },
    { L"objcpp", { L"mm", L"h" } },
    { L"ocaml", { L"ml", L"mli", L"mll", L"mly" } },
    { L"octave", { L"m" } },
    { L"parrot", { L"pir", L"pasm", L"pmc", L"ops", L"pod", L"pg", L"tg" } },
    { L"perl", { L"pl", L"pm", L"pm6", L"pod", L"t" } },
    { L"php", { L"php", L"phpt", L"php3", L"php4", L"php5", L"phtml" } },
    { L"pike", { L"pike", L"pmod" } },
    { L"plone", { L"pt", L"cpt", L"metadata", L"cpy", L"py" } },
    { L"puppet", { L"pp" } },
    { L"python", { L"py" } },
    { L"rake", { L"Rakefiles" } },
    { L"rs", { L"rs" } },
    { L"r", { L"R", L"Rmd", L"Rnw", L"Rtex", L"Rrst" } },
    { L"ruby", { L"rb", L"rhtml", L"rjs", L"rxml", L"erb", L"rake", L"spec" } },
    { L"rust", { L"rs" } },
    { L"salt", { L"sls" } },
    { L"sass", { L"sass", L"scss" } },
    { L"scala", { L"scala" } },
    { L"scheme", { L"scm", L"ss" } },
    { L"shell", { L"sh", L"bash", L"csh", L"tcsh", L"ksh", L"zsh" } },
    { L"smalltalk", { L"st" } },
    { L"sql", { L"sql", L"ctl" } },
    { L"stylus", { L"styl" } },
    { L"swift", { L"swift" } },
    { L"tcl", { L"tcl", L"itcl", L"itk" } },
    { L"tex", { L"tex", L"cls", L"sty" } },
    { L"tt", { L"tt", L"tt2", L"ttml" } },
    { L"vala", { L"vala", L"vapi" } },
    { L"vb", { L"bas", L"cls", L"frm", L"ctl", L"vb", L"resx" } },
    { L"velocity", { L"vm" } },
    { L"verilog", { L"v", L"vh", L"sv" } },
    { L"vhdl", { L"vhd", L"vhdl" } },
    { L"vim", { L"vim" } },
    { L"wsdl", { L"wsdl" } },
    { L"wadl", { L"wadl" } },
    { L"xml", { L"xml", L"dtd", L"xsl", L"xslt", L"ent" } },
    { L"yaml", { L"yaml", L"yml" } }
};

size_t get_lang_count() {
    return sizeof(langs) / sizeof(lang_spec_t);
}

wchar_t *make_lang_regex(wchar_t *ext_array, size_t num_exts) {
    int regex_capacity = 100;
    wchar_t *regex = (wchar_t *)ag_malloc(regex_capacity * sizeof(wchar_t));
    int regex_length = 3;
    int subsequent = 0;
	wchar_t *extension;
    size_t i;

    wcscpy(regex, L"\\.(");

    for (i = 0; i < num_exts; ++i) {
        extension = ext_array + i * SINGLE_EXT_LEN;
        int extension_length = wcslen(extension);
        while (regex_length + extension_length + 3 + subsequent > regex_capacity) {
            regex_capacity *= 2;
			regex = (wchar_t *)ag_realloc(regex, regex_capacity * sizeof(wchar_t));
        }
        if (subsequent) {
            regex[regex_length++] = '|';
        } else {
            subsequent = 1;
        }
        wcscpy(regex + regex_length, extension);
        regex_length += extension_length;
    }

    regex[regex_length++] = ')';
    regex[regex_length++] = '$';
    regex[regex_length++] = 0;
    return regex;
}

size_t combine_file_extensions(size_t *extension_index, size_t len, wchar_t **exts) {
    /* Keep it fixed as 100 for the reason that if you have more than 100
     * file types to search, you'd better search all the files.
     * */
    size_t ext_capacity = 100;
    (*exts) = (wchar_t *)ag_malloc(ext_capacity * SINGLE_EXT_LEN * sizeof(wchar_t));
    memset((*exts), 0, ext_capacity * SINGLE_EXT_LEN * sizeof(wchar_t));
    size_t num_of_extensions = 0;

    size_t i;
    for (i = 0; i < len; ++i) {
        size_t j = 0;
        const wchar_t *ext = langs[extension_index[i]].extensions[j];
        do {
            if (num_of_extensions == ext_capacity) {
                break;
            }
			wchar_t *pos = (*exts) + num_of_extensions * SINGLE_EXT_LEN * sizeof(wchar_t);
            wcsncpy(pos, ext, wcslen(ext));
            ++num_of_extensions;
            ext = langs[extension_index[i]].extensions[++j];
        } while (ext);
    }

    return num_of_extensions;
}
