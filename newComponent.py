import os

def create_component(category, component_name, project_root="."):
    
    valid_categories = ["brain", "drivers", "periodics", "utils"]

    print(os.getcwd())
    
    if category not in valid_categories:
        print("Invalid category, try again..")
        return

    include_path = os.path.join(project_root, "include", category)
    source_path = os.path.join(project_root, "source", category)

    print(include_path)
    print(source_path)

    os.makedirs(include_path, exist_ok=True)
    os.makedirs(source_path, exist_ok=True)

    #Create header file
    header_file_path = os.path.join(include_path, f"{component_name}.hpp")
    if not os.path.exists(header_file_path):
        with open(header_file_path, "w") as f:
            f.write(f"#ifndef {component_name.upper()}_HPP\n")
            f.write(f"#define {component_name.upper()}_HPP\n\n")
            f.write(f"// TODO: Add your code here\n\n")
            f.write(f"#endif // {component_name.upper()}_HPP\n")
        print(f"Created: {header_file_path}")
    else:
        print(f"File already exists: {header_file_path}")

    #Create source file
    source_file_path = os.path.join(source_path, f"{component_name}.cpp")
    if not os.path.exists(source_file_path):
        with open(source_file_path, "w") as f:
            f.write(f'#include "{category}/{component_name}.hpp"\n\n')
            f.write(f"// TODO: Add your code here\n")
        print(f"Created: {source_file_path}")
    else:
        print(f"File already exists: {source_file_path}")

category = input("Type the category of the new component (brain, drivers, periodics, utils): ").strip().lower()
component_name = input("Type the name of the new component: ").strip().lower()

if component_name[0].isalpha():
    create_component(category, component_name)
else:
    print("Invalid component name")

    
