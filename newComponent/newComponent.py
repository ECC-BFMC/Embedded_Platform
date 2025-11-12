import os
from difflib import get_close_matches

# --- ANSI color definitions ---
RESET = "\033[0m"
BOLD = "\033[1m"

FG_RED = "\033[31m"
FG_GREEN = "\033[32m"
FG_YELLOW = "\033[33m"
FG_BLUE = "\033[34m"
FG_CYAN = "\033[36m"
FG_WHITE = "\033[37m"

def create_component(category, component_name, want_serial_callback, project_root="."):
    
    existing_categories = ["brain", "drivers", "periodics", "utils"]

    path_EmbeddedPlatform = str(os.getcwd())
    last_backslash = str(os.getcwd())[::-1].index('\\')
    filename_Emb = path_EmbeddedPlatform[:len(path_EmbeddedPlatform)-last_backslash]
    filename_CMake = path_EmbeddedPlatform[:len(path_EmbeddedPlatform)-last_backslash] + "CMakeLists.txt"

    if category not in existing_categories:
        last_include = -1
        last_source = -1

        with open(filename_CMake, 'r') as file:
            lines = file.readlines()

        for x in lines:
            if '    ${CMAKE_CURRENT_SOURCE_DIR}/include/' in x:
                last_include = int(lines.index(x))
            if '    "${CMAKE_CURRENT_SOURCE_DIR}/source/' in x:
                last_source = int(lines.index(x))

        lines.insert(last_source + 1, '    "${CMAKE_CURRENT_SOURCE_DIR}/source/' + category + '/*.cpp"' + '\n')
        lines.insert(last_include + 2, '    ${CMAKE_CURRENT_SOURCE_DIR}/include/' + category + '\n')

        with open(filename_CMake, 'w') as file:
            file.writelines(lines)

    include_path = os.path.join(filename_Emb, "include", category)
    source_path = os.path.join(filename_Emb, "source", category)

    os.makedirs(include_path, exist_ok=True)
    os.makedirs(source_path, exist_ok=True)

    #Create header file
    header_file_path = os.path.join(include_path, f"{component_name}.hpp")
    if not os.path.exists(header_file_path):
        with open(header_file_path, "w") as f:
            f.write(f"#ifndef {component_name.upper()}_HPP\n")
            f.write(f"#define {component_name.upper()}_HPP\n\n")
            f.write(f"// TODO: Add your code here\n\n")
            if "periodics" == category:
                f.write(f"#include <chrono>\n")
                f.write(f"#include <utils/task.hpp>\n\n")
            f.write(f"namespace {category}\n")
            f.write('{\n')
            f.write('   /**\n')
            f.write('    * @brief Class ' + component_name + '\n')
            f.write('    *\n')
            f.write('    */\n')

            if "periodics" == category:
                f.write(f'    class C{component_name.capitalize()}: public utils::CTask\n')
                f.write('    {\n')
                f.write('        public:\n')
                f.write('            /* Constructor */\n')
                f.write(f'            C{component_name.capitalize()}(\n')
                f.write('                std::chrono::milliseconds f_period\n')
                f.write('            );\n')
            else:
                f.write(f'    class C{component_name.capitalize()}\n')
                f.write('    {\n')
                f.write('        public:\n')
                f.write('            /* Constructor */\n')
                f.write(f'            C{component_name.capitalize()}(\n')
                f.write('                int possible_argument\n')
                f.write('            );\n')

            f.write('            /* Destructor */\n')
            f.write(f'            ~C{component_name.capitalize()}();\n')

            if want_serial_callback_b:
                f.write(f'\n            void serialCallback{component_name.capitalize()}Command(char const * message, char * response);\n')

            f.write('\n        private:\n')
            f.write('            /* private variables & method member */\n\n')

            if "periodics" == category:
                f.write(f'\n            /* Run method */\n')
                f.write('            virtual void        _run();\n\n')
                f.write('            /** @brief Active flag  */\n')
                f.write('            bool m_isActive;\n\n')

            f.write('    }; // class ' + f'C{component_name.capitalize()}\n')
            f.write('}; // namespace '+ f'{category}\n\n')
            f.write(f"#endif // {component_name.upper()}_HPP\n")
            
        print(f"Created: {header_file_path}")

        with open(os.path.join(filename_Emb, "include", "main.hpp"), "a") as f:
            f.write(f'/* Header file for the {component_name.capitalize()} functionality */\n')
            f.write(f"#include <{category}/{component_name}.hpp>\n")
    else:
        print(f"File already exists: {header_file_path}")

    #Create source file
    source_file_path = os.path.join(source_path, f"{component_name}.cpp")
    if not os.path.exists(source_file_path):
        with open(source_file_path, "w") as f:
            f.write(f'#include "{category}/{component_name}.hpp"\n\n')
            f.write(f"// TODO: Add your code here\n")
            f.write(f"namespace {category}\n")
            f.write('{\n')
            f.write('   /**\n')
            f.write('    * @brief Class constructor ' + component_name + '\n')
            f.write('    *\n')
            f.write('    */\n')

            if "periodics" == category:
                f.write(f'    C{component_name.capitalize()}::C{component_name.capitalize()}(\n')
                f.write('        std::chrono::milliseconds f_period\n')
                f.write('    )\n')
                f.write('    : utils::CTask(f_period)\n')
                f.write('    {\n')
            else:
                f.write(f'    C{component_name.capitalize()}::C{component_name.capitalize()}(int possible_argument)\n')
                f.write('    {\n')
            
            f.write('        /* constructor behaviour */\n')
            f.write('    }\n\n')
            f.write(f'    /** @brief  C{component_name.capitalize()} class destructor\n')
            f.write('     */\n')
            f.write(f'    C{component_name.capitalize()}::~C{component_name.capitalize()}()\n')
            f.write('    {\n')
            f.write('    };\n')

            if want_serial_callback_b:
                f.write(f'\n    void C{component_name.capitalize()}::serialCallback{component_name.capitalize()}Command(char const * message, char * response)\n')
                f.write('    {\n')
                f.write('     \n')
                f.write('    }\n')

            if "periodics" == category:
                f.write(f'\n    /* Run method */\n')
                f.write(f'    void C{component_name.capitalize()}::_run()\n')
                f.write('    {\n')
                f.write('        /* Run method behaviour */\n')
                f.write('        if(!m_isActive) return;\n')
                f.write('    }\n')
                
            f.write('\n}; // namespace ' + f'{category}')

        print(f"Created: {source_file_path}")

        with open(os.path.join(filename_Emb, "source", "main.cpp"), "r") as f:
            lines = f.readlines()

        map_start = 0

        for i, line in enumerate(lines):
            if '/* USER NEW COMPONENT BEGIN */' in line:
                if "periodics" == category:
                    lines.insert(i + 1, f'{category}::C{component_name.capitalize()} g_{component_name}(g_baseTick * NO_OF_MILISECONDS);\n')
                else:
                    lines.insert(i + 1, f'{category}::C{component_name.capitalize()} g_{component_name}(possible_argument);\n')
            
            if 'CSerialMonitor::CSerialSubscriberMap' in line and want_serial_callback_b:
                map_start = i
            elif map_start and line.strip() == '};':
                new_entry = f'    {{"{component_name}", mbed::callback(&g_{component_name}, &{category}::C{component_name.capitalize()}::serialCallback{component_name.capitalize()}Command)}},\n'
                lines.insert(i, new_entry)
                map_start = 0

            if '// USER NEW PERIODICS BEGIN' in line and "periodics" == category:
                lines.insert(i + 1, f'    &g_{component_name},\n')
                break
        
        with open(os.path.join(filename_Emb, "source", "main.cpp"), 'w') as file:
            file.writelines(lines)

    else:
        print(f"File already exists: {source_file_path}")

def validate_category(category_input, existing_categories):
    category = category_input.strip().lower()

    if category == '':
        return None, None
    
    if category in existing_categories:
        return category, False
    
    close_matches = get_close_matches(category, existing_categories, n=1, cutoff=0.6)
    
    if close_matches:
        suggestion = close_matches[0]
        print(f"\n{FG_YELLOW}Category '{category}' not found in existing categories.{RESET}")
        print(f"{FG_CYAN}Did you mean '{suggestion}'?{RESET}")
        
        response = input(f"Use '{suggestion}' instead? ({FG_GREEN}{BOLD}y{RESET}/{FG_RED}{BOLD}n{RESET}): ").strip().lower()
        
        if response in ['y', 'yes']:
            print(f"{FG_YELLOW}Using existing category: '{suggestion}'{RESET}")
            return suggestion, False
        else:
            print(f"➕ Creating new category: '{category}'")
            return category, True
    else:
        print(f"\n📋 Existing categories: {', '.join(existing_categories)}")
        print(f"➕ '{category}' is a new category.")
        
        response = input(f"Create new category '{category}'? ({FG_GREEN}{BOLD}y{RESET}/{FG_RED}{BOLD}n{RESET}): ").strip().lower()
        
        if response in ['y', 'yes']:
            print(f"✅ Creating new category: '{category}'")
            return category, True
        else:
            return None, None

if __name__ == "__main__":
    existing_categories = ["brain", "drivers", "periodics", "utils"]

    print()
    print(FG_GREEN + BOLD + "=" * 60 + RESET)
    print(FG_CYAN + BOLD + "Component Generator" + RESET)
    print(FG_GREEN + BOLD + "=" * 60 + RESET)

    print(f"\n{FG_WHITE}{BOLD}Existing categories:{RESET} {FG_YELLOW}{', '.join(existing_categories)}{RESET}\n")

    category_input = input(f"{FG_GREEN}{BOLD}Enter the category of the new component:{RESET} ").strip()
    category, is_new = validate_category(category_input, existing_categories)
    
    if category is None:
        print(f"{FG_RED}{BOLD}Operation cancelled.{RESET}")
        exit(0)

    print()

    component_name = input("Type the name of the new component: ").strip().lower()

    if not component_name or not component_name[0].isalpha():
        print(f"{FG_RED}{BOLD}Invalid request: component name must start with a letter.{RESET}")
        exit(1)
    
    print()

    want_serial_callback = input(f"Do you want a serial callback command for this component?\nSo you can interact with her like this {FG_GREEN}{BOLD}#{RESET}{FG_CYAN}{BOLD}component{RESET}:{FG_YELLOW}{BOLD}args{RESET};; ?({FG_GREEN}{BOLD}y{RESET}/{FG_RED}{BOLD}n{RESET}): ").strip().lower()
    print()

    want_serial_callback_b = want_serial_callback in ['y', 'yes']

    print(FG_BLUE + BOLD + "=" * 60 + RESET)
    print(FG_WHITE + BOLD + "Summary:" + RESET)
    print(f"   {FG_CYAN}Category:{RESET} {category} {'(new)' if is_new else '(existing)'}")
    print(f"   {FG_CYAN}Component:{RESET} {component_name}")
    print(f"   {FG_CYAN}Serial callback:{RESET} {'Yes' if want_serial_callback_b else 'No'}")
    print(FG_BLUE + BOLD + "=" * 60 + RESET)

    confirm = input(f"{BOLD}Proceed with creation?{RESET} ({FG_GREEN}{BOLD}y{RESET}/{FG_RED}{BOLD}n{RESET}): ").strip().lower()

    if confirm in ['y', 'yes']:
        create_component(category, component_name, want_serial_callback_b)
        print(f"{FG_GREEN}{BOLD}Component created successfully!{RESET}")
    else:
        print(f"{FG_RED}{BOLD}Operation cancelled.{RESET}")