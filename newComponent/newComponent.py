import os

def create_component(category, component_name, project_root="."):
    
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
                f.write(f'            C{component_name.capitalize()}(\n\n')
                f.write('            );\n')

            f.write('            /* Destructor */\n')
            f.write(f'            ~C{component_name.capitalize()}();\n')
            f.write('        private:\n')
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
                f.write(f'    C{component_name.capitalize()}::C{component_name.capitalize()}()\n')
                f.write('    {\n')
            
            f.write('        /* constructor behaviour */\n')
            f.write('    }\n\n')
            f.write(f'    /** @brief  C{component_name.capitalize()} class destructor\n')
            f.write('     */\n')
            f.write(f'    C{component_name.capitalize()}::~C{component_name.capitalize()}()\n')
            f.write('    {\n')
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

        # print(lines)

        for i, line in enumerate(lines):
            if '/* USER NEW COMPONENT BEGIN */' in line:
                if "periodics" == category:
                    lines.insert(i + 1, f'{category}::{component_name} g_{component_name.lower()}(g_baseTick * NO_OF_MILISECONDS);\n')
                else:
                    lines.insert(i + 1, f'{category}::{component_name} g_{component_name.lower()}(possible_argument);\n')

            if '// USER NEW PERIODICS BEGIN' in line and "periodics" == category:
                print("Periodics")
                lines.insert(i + 1, f'    &g_{component_name.lower()},\n')
                break
        
        with open(os.path.join(filename_Emb, "source", "main.cpp"), 'w') as file:
            file.writelines(lines)

    else:
        print(f"File already exists: {source_file_path}")

category = input("Type the category of the new component (brain, drivers, periodics, utils or the name for the new one): ").strip().lower()
component_name = input("Type the name of the new component: ").strip().lower()

if component_name[0].isalpha():
    create_component(category, component_name)
else:
    print("Invalid component name")