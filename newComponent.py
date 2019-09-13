from sys import argv
import os
import argparse
import datetime

include_dir="include\\"
source_dir="src\\"


header_note=('/**\n'+
'******************************************************************************\n'+
'* @file    %s\n'+
'* @author  RBRO/PJ-IU\n'+
'* @version V1.0.0\n'+
'* @date    %s\n'+
'* @brief   \n'+
'******************************************************************************\n'+
'*/\n')


def check(compName,subdirectory,newDirectory):
        
    if newDirectory:
        isNotExist = not os.path.exists(include_dir +subdirectory+compName) and not os.path.exists(source_dir +subdirectory+compName)
        return isNotExist
    else:

        lowcaseName=compName.lower()
        isNotExist = not os.path.exists(include_dir +subdirectory+lowcaseName+".hpp") and not os.path.exists(include_dir +subdirectory+lowcaseName+".inl") and not os.path.exists(source_dir +subdirectory+lowcaseName+".cpp")
        return isNotExist
    return False


def main():
    parser = argparse.ArgumentParser(description="This script create a the directories, the source and the include files for the new component.")
    parser.add_argument("-c","--component",help="Get the name of the new component.",required=True,dest="component")
    parser.add_argument("-sb","--subdirectory",help="Get the path in for the components.",dest="subdir")
    parser.add_argument("-n","--newDirectory",help="Make new directory with the component name in the subdirectory.",action='store_true',dest="isCreateDir")

    parser_result=parser.parse_args()


    print("Component name:",parser_result.component)
    print("Subdirectory:",parser_result.subdir)
    print("Create a new directory:",parser_result.isCreateDir)
    if (parser_result.subdir is None):
        parser_result.subdir=""
    elif (parser_result.subdir[-1]!='\\' or parser_result.subdir[-1]!='/'):
        parser_result.subdir+='\\'
    
    isNotExist=check(parser_result.component,parser_result.subdir,parser_result.isCreateDir)
    if isNotExist:
        today = datetime.date.today()
        source_path = source_dir + parser_result.subdir
        include_path = include_dir + parser_result.subdir
        src_include = parser_result.subdir
        if parser_result.isCreateDir:
            os.makedirs(include_dir + parser_result.subdir + parser_result.component)
            os.makedirs(source_dir + parser_result.subdir + parser_result.component)
            source_path = source_dir + parser_result.subdir + parser_result.component+"\\"
            include_path = include_dir + parser_result.subdir + parser_result.component+"\\"
            src_include= parser_result.subdir + parser_result.component+"/"
        if not os.path.exists(include_dir +parser_result.subdir):
            os.makedirs(include_dir +parser_result.subdir)
        if not os.path.exists(source_dir +parser_result.subdir):
            os.makedirs(source_dir +parser_result.subdir)
        lowercaseName=parser_result.component.lower()
        file = open(include_path+lowercaseName+".hpp", 'w')
        # note=header_note%lowercaseName
        file.writelines(header_note%(lowercaseName+".hpp",today))
        file.writelines("#ifndef " + lowercaseName.upper() + "_H\n")
        file.writelines("#define " + lowercaseName.upper() + "_H\n")
        file.writelines('#include "' + lowercaseName +'.inl"\n')
        file.writelines("#endif // " + lowercaseName.upper() + "_H\n")
        file.close()
        file = open(include_path+lowercaseName + ".inl", 'w')
        file.writelines("\n")
        file.close()
        file = open(source_path+lowercaseName + ".cpp", 'w')
        file.writelines("#include <" +src_include + lowercaseName +".hpp>\n")
        file.close()
    else:
        print("component already exists")


if __name__=="__main__":
    main()