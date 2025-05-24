# 项目名称：ImageProcessor

## 简介
ImageProcessor 是一个基于 C 语言的图像处理工具，使用 stb_image 库读取和写入 PNG/JPG 图像，支持多种滤镜和图像处理功能。

## 功能
- 读取和写入 PNG/JPG 图像
- 灰度化 (Grayscale)
- 反色 (Invert)
- 模糊滤镜 (Blur)
- ASCII 字符画生成 (ASCII Art)
- 边缘检测 (Sobel 算子)
- 图像旋转 (矩阵变换)
- 批量处理 (Batch Processing)

## 新增功能
- **灰度化 (Grayscale)**: 通过命令行参数读取图像并生成灰度图像。

## 依赖
- C 编译器 (GCC/Clang/Visual Studio)
- `stb_image.h` 和 `stb_image_write.h`（第三方库）

## 构建
1. 安装依赖。
2. 在项目根目录下(./cpl_design)运行以下powershell命令：
- 如果你使用MinGW:
```powershell
mingw32-make
```

- 如果你使用 MSYS2、Git Bash 或安装了 make：
```
make
```

1. 可执行文件将在 `bin/` 目录下生成。

## 项目结构
```
.
├── src                     // 源代码目录
│   ├── main.c              // 主程序入口
│   ├── image.c             // 图像加载和保存
│   ├── filters.c           // 滤镜效果实现
│   ├── ascii_art.c         // ASCII 字符画生成
│   ├── edge.c              // 边缘检测实现
│   ├── rotate.c            // 图像旋转功能
│   └── batch.c             // 批量处理功能
├── include                 // 头文件目录
│   ├── image.h             // 图像处理相关声明
│   ├── filters.h           // 滤镜函数声明
│   ├── ascii_art.h         // ASCII 艺术相关声明
│   ├── edge.h              // 边缘检测相关声明
│   ├── rotate.h            // 旋转功能相关声明
│   └── batch.h             // 批处理相关声明
├── third_party             // 第三方库
│   └── stb                 // stb 单头文件库
│       ├── stb_image.h     // 图像加载库
│       └── stb_image_write.h // 图像保存库
├── obj                     // 编译生成的对象文件
│   ├── main.o
│   ├── image.o
│   ├── filters.o
│   ├── ascii_art.o
│   ├── edge.o
│   ├── rotate.o
│   └── batch.o
├── bin                     // 可执行文件输出
│   └── ImageProcessor.exe  // 程序可执行文件
├── image.jpg               // 测试图像
├── .clang-format           // 格式化工具配置文件
├── Makefile                // 项目构建配置
└── README.md               // 项目说明
```

## 使用示例

### 命令行运行方式（推荐）
```bash
# 在项目根目录下运行
bin/ImageProcessor input.jpg output.jpg
```
其中 `input.jpg` 是输入图像文件，`output.jpg` 是生成的灰度图像文件。

### 运行问题及解决方法

**问题：** 程序窗口一闪而过，快速关闭

**原因：** 
- 没有提供命令行参数
- 控制台应用程序执行完毕后自动关闭窗口

**解决方法：**

1. **使用命令提示符/PowerShell运行程序**
   ```bash
   cd D:\path\to\project
   bin\ImageProcessor.exe input.jpg output.jpg
   ```

2. **创建批处理文件 (run.bat)**
   ```bat
   @echo off
   bin\ImageProcessor.exe input.jpg output.jpg
   pause
   ```
   双击 run.bat 执行，`pause` 命令会在程序结束后等待用户按键

3. **检查处理结果**
   - 查看是否生成了指定的输出文件
   - 检查输出文件是否正确处理（如已转为灰度）

### 命令行参数说明

```
ImageProcessor <input_image> <output_image>
```

- `<input_image>`: 待处理的图像文件路径（支持 jpg, png, bmp 等格式）
- `<output_image>`: 处理后输出的图像文件路径

## 许可证
MIT License