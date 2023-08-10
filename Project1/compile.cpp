#include<iostream>
#include<string.h>
#include<filesystem>
#include<fstream>
#include<cstdlib>
#include<unordered_map>
#include<vector>
/*
    shader platform profile filename1 filename2 varyingdef
    texture filename1 filename2
    geometry filename1
*/

enum compile_mode{
    shader = 1,
    texture = 2,
    geometry = 3
};
static std::unordered_map<std::string,int> mode_map = {{"shader",shader},{"texture",texture},{"geometry",geometry}};
std::vector<std::pair<std::string,std::string>> command_list = {
    std::pair<std::string,std::string>("platform",""),/*0*/
    std::pair<std::string,std::string>("profile",""),/*1*/
    std::pair<std::string,std::string>("filename1",""),/*2*/
    std::pair<std::string,std::string>("filename2",""),/*3*/
    std::pair<std::string,std::string>("varyingdef","")/*4*/
    };

void error_log(){
    std::cout << "You entered an incorrect command" << std::endl;
    std::cout << "-----------command-----------" << std::endl;
    std::cout << "1.shader platform profile filename1 filename2 varyingdef" << std::endl;
    std::cout << "2.texture platform filename" << std::endl;
    std::cout << "3.geometry platform filename" << std::endl;
}    

int main(int argc,char** argv){
    auto mode = mode_map.find(argv[1]);
    if(mode == mode_map.end()){
        error_log();
        return -1;
    }

    if(argc == 7){
        command_list[0].second = argv[2];
        command_list[1].second = argv[3];
        command_list[2].second = argv[4];
        command_list[3].second = argv[5];
        command_list[4].second = argv[6];
    }else if(argc == 4){
        command_list[2].second = argv[2];
        command_list[3].second = argv[3];
    }else if(argc == 3){
        command_list[2].second = argv[2];
    }
    else{
        error_log();
    }

    std::ifstream file1;
    std::ifstream file2;

    switch (mode->second)
    {
    case 1 :
        file1.open("shader\\" + command_list[2].second + ".sc", std::ios::in);
        file2.open("shader\\" + command_list[3].second + ".sc", std::ios::in);
        if (file1 && file2) {
            file1.close();
            file2.close();
            std::string vs_command = ".\\include\\bgfx\\.build\\win64_vs2022\\bin\\shadercDebug.exe";
            vs_command.append(" -f shader\\" + command_list[2].second + ".sc");
            vs_command.append(" -o shader_compile\\" + command_list[2].second + ".bin");
            vs_command.append(" --platform " + command_list[0].second);
            vs_command.append(" --type vertex");
            vs_command.append(" --verbose 0 --profile");
            vs_command.append(strcmp(command_list[1].second.c_str(),"glsl")==0 ? " 430" : " s_5_0 -O 3");
            vs_command.append(" --varyingdef shader\\" + command_list[4].second + ".sc");
            vs_command.append(" -i include\\bgfx\\src -i include\\bgfx\\examples\\common");
            std::cout << vs_command << std::endl;

            std::string fs_command = ".\\include\\bgfx\\.build\\win64_vs2022\\bin\\shadercDebug.exe";
            fs_command.append(" -f shader\\" + command_list[3].second + ".sc");
            fs_command.append(" -o shader_compile\\" + command_list[3].second + ".bin");
            fs_command.append(" --platform " + command_list[0].second);
            fs_command.append(" --type fragment");
            fs_command.append(" --verbose 0 --profile");
            fs_command.append(strcmp(command_list[1].second.c_str(),"glsl")==0 ? " 430" : " s_5_0 -O 3");
            fs_command.append(" --varyingdef shader\\" + command_list[4].second + ".sc");
            fs_command.append(" -i include\\bgfx\\src -i include\\bgfx\\examples\\common");
            std::cout << fs_command << std::endl;

            std::system(vs_command.c_str());
            std::system(fs_command.c_str());

            std::cout << "compile shader successfully!!!" << std::endl;
        }
        else std::cout << "can't open file" << std::endl;
        break;
    case 2 : 
        file1.open("resource\\textures\\" + command_list[2].second, std::ios::in);
        if(file1){
            file1.close();
            std::string texture_command = ".\\include\\bgfx\\.build\\win64_vs2022\\bin\\texturecDebug.exe";
            texture_command.append(" -f resource\\textures\\" + command_list[2].second);
            texture_command.append(" -o resource\\textures_compile\\" + command_list[3].second);
            std::cout << texture_command << std::endl;

            std::system(texture_command.c_str());

            std::cout << "compile texture successfully!!!" << std::endl;
        }
        else std::cout << "can't open file" << std::endl;

        break;
    case 3 :
        file1.open("resource\\meshes\\" + command_list[2].second, std::ios::in);
        if(file1){
            file1.close();
            std::string geometry_command = ".\\include\\bgfx\\.build\\win64_vs2022\\bin\\geometrycDebug.exe";
            geometry_command.append(" -f resource\\meshes\\" + command_list[2].second);
            geometry_command.append(" -o resource\\meshes_compile\\" + command_list[2].second.substr(0,command_list[2].second.find('.')) + ".bin");
            std::cout << geometry_command << std::endl;

            std::system(geometry_command.c_str());

            std::cout << "compile geometry successfully!!!" << std::endl;
        }
        else std::cout << "can't open file" << std::endl;
        
        break;
    }

    return 0;
}
