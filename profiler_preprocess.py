import glob

skipped_files = [
    "closure.cpp",
    "glcontext.cpp",
    "keycodes.cpp",
    "keymouseconditions.cpp",
    "opengl.cpp",
    "sadhash.cpp",
    "sadluvcolor.cpp",
    "sadmutex.cpp",
    "sadscopedlock.cpp",
    "sadsemaphore.cpp",
    "sadsleep.cpp",
    "sadstring.cpp",
    "sadthread.cpp",
    "sadthreadexecutablefunction.cpp",
    "sadwstring.cpp",
    "settings.cpp",
    "window.cpp",
    "db/dbtypename.cpp",
    "db/dbvariant.cpp",
    "db/dblink.cpp",
    "clipboard.cpp",
    "resource/abstractlink.cpp",
    "bindable.cpp",
    "classmetadata.cpp",
    "classmetadatacastfunction.cpp",
    "classmetadatacontainer.cpp",
    "clipboard.cpp"
]

def find_and_insert_header(string): 
    line = 0;
    is_in_headers = True
    strings  = string.splitlines()
    last_non_empty_string = 0
    while (is_in_headers):
        current_line = strings[line]
        current_line_stripped = current_line.strip()
        if (len(current_line_stripped) != 0):
            if (current_line[0] != '#'):
                is_in_headers = False
            else:
                last_non_empty_string = line
        if (is_in_headers):
            line = line + 1
    if (is_in_headers):
        strings.append("#include \"opticksupport.h\"")
    else:
        line = last_non_empty_string + 1
        strings.insert(line, "#include \"opticksupport.h\"")
    return "\n".join(strings)

def find_and_insert_events(string):
    strings  = string.splitlines()
    line = 0
    while (line != len(strings)):
        if (strings[line] == '{'):
            strings.insert(line + 1, "    PROFILER_EVENT;")
            line = line + 1
        line = line + 1
    return "\n".join(strings)
    
if __name__ == '__main__':
    files = glob.glob("**", root_dir="src", recursive=True)
    files = map(lambda s : s.replace("\\", "/"), files)
    files = filter(lambda s : s.endswith(".cpp"), files)
    files = filter(lambda s : not (s.startswith("3rdparty/") or s.startswith("os/") or s.startswith("util/") or s.startswith("cli/") or s.startswith("imageformats/")), files)
    files = filter(lambda s : not (s in skipped_files), files)

    files = list(files)
    content = ""
    for file in files:
        print("Processing", file)
        with open("src/" + file, 'r') as file_handler:
            content = file_handler.read()

        content = find_and_insert_header(content)
        content = find_and_insert_events(content)
        with open("src/" + file, 'w') as file_handler:
            file_handler.write(content + "\n")