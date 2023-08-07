#include<iostream>
#include<string.h>
#include<filesystem>
#include<fstream>
#include<cstdlib>
/*
    shader platform profile filename
    geometry platform filename
    texture platform filename
*/

int main(int argc,char** argv){
    std::string mode = *(argv + 1);
    switch (mode)
    {
    case "shader" :
        std::string platform = *(argv + 2);
        std::string profile = *(argv + 3);
        std::string filename = std::string(*(argv + 4));
        std::string vs_filename = "vs_" + filename;
        std::string fs_filename = "fs_" + filename;
        std::string vs_path = "shader\\" + vs_filename + ".sc";
        std::string fs_path = "shader\\" + fs_filename + ".sc";
        std::ifstream vs_file, fs_file;
        vs_file.open(vs_path, std::ios::in);
        fs_file.open(fs_path, std::ios::in);
        if (vs_file && fs_file) {
            vs_file.close();
            fs_file.close();
            std::string vs_command = ".\\include\\bgfx\\.build\\win64_vs2022\\bin\\shadercDebug.exe";
            vs_command.append(" -f shader\\" + vs_filename + ".sc");
            vs_command.append(" -o shader_compile\\" + vs_filename + ".bin");
            vs_command.append(" --platform " + platform);
            vs_command.append(" --type vertex");
            vs_command.append(" --verbose 0");
            vs_command.append(" --profile " + profile);
            vs_command.append(" --varyingdef shader\\" + filename + "_varying.def.sc");
            vs_command.append(" -i include\\bgfx\\src -i include\\bgfx\\examples\\common");
            std::cout << vs_command << std::endl;

            std::string fs_command = ".\\include\\bgfx\\.build\\win64_vs2022\\bin\\shadercDebug.exe";
            fs_command.append(" -f shader\\" + fs_filename + ".sc");
            fs_command.append(" -o shader_compile\\" + fs_filename + ".bin");
            fs_command.append(" --platform " + platform);
            fs_command.append(" --type fragment");
            fs_command.append(" --verbose 0");
            fs_command.append(" --profile " + profile);
            fs_command.append(" --varyingdef shader\\" + filename + "_varying.def.sc");
            fs_command.append(" -i include\\bgfx\\src -i include\\bgfx\\examples\\common");
            std::cout << fs_command << std::endl;

            std::system(vs_command.c_str());
            std::system(fs_command.c_str());

            std::cout << "compile shader successfully!!!" << std::endl;
        }
        else {
            std::cout << "can't open file" << std::endl;
        }
        break;
    case "geometry" : 
        std::cout << "compile geometry successfully!!!" << std::endl;
        break;
    case "texture" :
        std::cout << "compile texture successfully!!!" << std::endl;
        break;
    }

    return 0;
}
