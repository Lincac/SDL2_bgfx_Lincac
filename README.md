# 使用依赖  
SDL2 --https://www.libsdl.org/  
bgfx --https://github.com/bkaradzic/bgfx  

# 项目构建  
SDL2的配置只需要它的include和lib文件就好,记得dll文件要放在项目.exe文件相同目录下--https://github.com/libsdl-org/SDL/releases/download/release-2.28.2/SDL2-devel-2.28.2-VC.zip    

bgfx的配置也比较简单,可以直接参考它的网页--https://bkaradzic.github.io/bgfx/build.html  
使用visual studio构建完成之后,我们需要其中的几个include、lib以及作者写的工具，一般只需要下面几个目录就好：  
bgfx\include  
bimg\include  
bx\include  
bgfx\examples\common  
bimg\3rdparty  
bx\include\compat\msvc （msvs是给微软的C++编辑器用的，如果你用的其他平台需要使用对应平台的文件目录）    

lib也只需要添加你需要的就好，一般有下面几个（我这里因为使用的是Debug模式，所以生成的lib文件是xxDebug.lib）：  
bgfxDebug.lib  
bxDebug.lib  
bimgDebug.lib  
bimg_decodeDebug.lib  

# 关于shader编译  
因为bgfx使用的是.bin格式的shader文件，因此我们需要编译对应的版本，不过作者已经给我们提供好了工具，具体文件在也是在lib的目录下，名字叫shadercDebug，可以使用这个exe来对shader文件进行编译，命令如下：  
shadercDebug.exe -f filename.sc -o filename.bin --platform x --type x --verbose x --profile x --varyingdef varying.def.sc  
[选项]  
-f 需要编译的文件  
-o 编译输出的文件  
--platform 编译平台：windows、linux  
--verbose 
        0 ： 不输出任何日志信息，包括进度条、loss、acc等。  
        1 ： 输出带进度条的日志信息，可以看到编译的过程和结果。  
        2 ： 输出不带进度条的日志信息，只显示每一步的结果。  
--profile 编译使用的着色语言有glsl、hlsl等等，具体可以去shader.mk文件中查看  
        glsl ： 430  
        hlsl ： s_5_0 -O 3  
--varyingdef 这里面定义的是着色器的输入和输入信息  

项目中我自己写了个简易的编辑程序，执行：.\compile_shader.exe windows 430 filename  
具体可以对比shader文件里面看文件  