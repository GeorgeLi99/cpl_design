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
- **灰度化 (Grayscale)**: 将彩色图像转换为灰度图像
- **反色处理 (Invert)**: 将图像颜色反转
- **高斯模糊 (Blur)**: 对图像应用高斯模糊效果
- **图像旋转 (Rotate)**: 对图像进行旋转处理

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
│   ├── main.c              // 主程序入口，整合所有功能模块
│   ├── image.c             // 图像加载和保存功能实现
│   ├── filters.c           // 滤镜效果实现（灰度、反色、模糊）
│   ├── ascii_art.c         // ASCII 字符画生成
│   ├── edge.c              // 边缘检测实现
│   ├── rotate.c            // 图像旋转功能
│   └── batch.c             // 批量处理功能
├── include                 // 头文件目录
│   ├── image.h             // 图像处理相关声明（加载、保存函数）
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

### 命令行运行方式
```bash
# 基本用法
bin/ImageProcessor <input_image> [output_dir]

# 示例1: 只指定输入图像，结果保存在当前目录
bin/ImageProcessor test.jpg

# 示例2: 指定输入图像和输出目录
bin/ImageProcessor test.jpg output_folder

# 示例3: 批量处理模式，处理batch_input目录中的所有图像
bin/ImageProcessor --batch
```

### 处理结果
程序会自动对输入图像应用四种处理效果，并分别保存为以下文件：
- `grayscale_output.jpg` - 灰度效果
- `blur_output.jpg` - 模糊效果（高斯模糊）
- `invert_output.jpg` - 反色效果
- `rotate_output.jpg` - 旋转效果

### 运行问题及解决方法

**问题：** 程序窗口一闪而过，快速关闭

**原因：** 
- 没有提供命令行参数
- 控制台应用程序执行完毕后自动关闭窗口

**解决方法：**

1. **使用命令提示符/PowerShell运行程序**
   ```bash
   cd D:\path\to\project
   bin\ImageProcessor.exe test.jpg
   ```

2. **创建批处理文件 (run.bat)**
   ```bat
   @echo off
   bin\ImageProcessor.exe test.jpg output
   pause
   ```
   双击 run.bat 执行，`pause` 命令会在程序结束后等待用户按键

3. **检查处理结果**
   - 查看是否生成了输出文件（grayscale_output.jpg, blur_output.jpg 等）
   - 检查输出文件是否正确处理

### 命令行参数说明

```
ImageProcessor <input_image> [output_dir]
ImageProcessor --batch
```

- `<input_image>`: 待处理的图像文件路径（支持 jpg, png, bmp 等格式）
- `[output_dir]`: 可选参数，指定处理后图像的保存目录，默认为当前目录("./"）
- `--batch`: 批量处理模式，处理 `batch_input` 目录中的所有图像，并将结果保存在 `batch_output` 目录下

### 批量处理模式

批量处理模式会处理 `batch_input` 目录中的所有图片文件，并将处理后的结果保存在 `batch_output` 目录的不同子文件夹中：

- `batch_output/grayscale/` - 灰度处理后的图像
- `batch_output/blur/` - 模糊处理后的图像
- `batch_output/invert/` - 反色处理后的图像
- `batch_output/rotate/` - 旋转处理后的图像
- `batch_output/ascii/` - ASCII字符画文件

每个图像会被处理并保存为对应的输出文件，文件名格式为 `原文件名_处理类型.扩展名`。

**使用步骤：**
1. 创建 `batch_input` 目录（如果不存在）
2. 将要处理的图像文件放入 `batch_input` 目录
3. 执行命令 `bin/ImageProcessor --batch`
4. 查看 `batch_output` 目录中的处理结果

## 许可证
MIT License