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

## 功能详情

### 基本图像处理
- **灰度化 (Grayscale)**: 将彩色图像转换为灰度图像，使用RGB加权平均法（R:0.299, G:0.587, B:0.114）
- **反色处理 (Invert)**: 将图像颜色反转，实现负片效果
- **高斯模糊 (Blur)**: 对图像应用高斯模糊效果，可调整模糊半径
- **图像旋转 (Rotate)**: 对图像进行旋转处理，使用矩阵变换

### 高级功能
- **边缘检测 (Edge Detection)**: 使用Sobel算子进行边缘检测，结合双阈值滞后处理提高边缘连续性
- **ASCII字符画 (ASCII Art)**: 将图像转换为ASCII字符画，支持多种风格和分辨率
  - 简单风格: 使用6种ASCII字符表示不同亮度
  - 扩展风格: 使用13种字符提供更细腻的灰度层次
  - 块状风格: 使用ASCII兼容字符构建清晰的边界
  - 经典风格: 使用传统字符集，保证全平台兼容性
  
- **批量处理 (Batch Processing)**: 批量处理目录中的所有图像文件，自动应用所有效果

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

### 目录布局
```
.
├── src/                    // 源代码目录
│   ├── main.c              // 主程序入口，整合所有功能模块
│   ├── image.c             // 图像加载和保存功能实现
│   ├── filters.c           // 滤镜效果实现（灰度、反色、模糊）
│   ├── ascii_art.c         // ASCII 字符画生成
│   ├── edge.c              // 边缘检测实现（Sobel算子）
│   ├── rotate.c            // 图像旋转功能
│   └── batch.c             // 批量处理功能
│
├── include/                // 头文件目录
│   ├── image.h             // 图像处理相关声明（加载、保存函数）
│   ├── filters.h           // 滤镜函数声明
│   ├── ascii_art.h         // ASCII 艺术相关声明
│   ├── edge.h              // 边缘检测相关声明
│   ├── rotate.h            // 旋转功能相关声明
│   └── batch.h             // 批处理相关声明
│
├── third_party/            // 第三方库
│   └── stb/                // stb 单头文件库
│       ├── stb_image.h     // 图像加载库
│       └── stb_image_write.h // 图像保存库
│
├── obj/                    // 编译生成的对象文件
│   ├── main.o
│   ├── image.o
│   ├── filters.o
│   ├── ascii_art.o
│   ├── edge.o
│   ├── rotate.o
│   └── batch.o
│
├── bin/                    // 可执行文件输出
│   └── ImageProcessor.exe  // 程序可执行文件
│
├── batch_input/            // 批处理输入目录（存放待处理图像）
│
├── batch_output/           // 批处理输出目录
│   ├── grayscale/          // 灰度处理结果
│   ├── blur/               // 模糊处理结果
│   ├── invert/             // 反色处理结果
│   ├── rotate/             // 旋转处理结果
│   ├── edge/               // 边缘检测结果
│   └── ascii/              // ASCII字符画
│
├── image.jpg               // 测试图像
├── lenna.png               // 经典测试图像
├── .clang-format           // 代码格式化配置文件
├── Makefile                // 项目构建配置
└── README.md               // 项目说明文档
```

### 模块说明

#### 核心模块
- **image**: 图像的加载和保存，封装了stb_image库的功能
- **filters**: 基础图像滤镜功能，包括灰度化、反色和高斯模糊
- **edge**: 边缘检测功能，使用增强型Sobel算子实现
- **rotate**: 图像旋转功能，使用矩阵变换实现
- **ascii_art**: ASCII字符画生成，支持多种字符集和风格
- **batch**: 批量处理功能，可处理目录中的所有图像

#### 编译与构建
- **Makefile**: 定义编译规则和目标
- **obj/**: 存放编译过程中生成的目标文件
- **bin/**: 存放最终生成的可执行程序

#### 输入输出
- **batch_input/**: 批处理模式的输入目录，存放待处理的图像
- **batch_output/**: 批处理模式的输出目录，按不同处理类型分类存储结果

### 核心算法示例

#### 灰度转换
```c
// 使用加权平均法进行灰度转换
float gray_value = 0.299f * r + 0.587f * g + 0.114f * b;
```

#### Sobel边缘检测
```c
// 水平梯度算子 Gx
// [-1 0 1]
// [-2 0 2]
// [-1 0 1]

// 垂直梯度算子 Gy
// [-1 -2 -1]
// [ 0  0  0]
// [ 1  2  1]

// 计算梯度幅值
int magnitude = (int)sqrt(gx * gx + gy * gy);
```

#### ASCII字符画映射
```c
// 将像素亮度映射到ASCII字符
const char *ascii_chars = " .:=#"; // 块状ASCII兼容字符集
int char_index = (int)(brightness_value * (ascii_chars_len - 1));
```

### 文件说明

#### main.c
主程序入口，处理命令行参数，协调各功能模块的调用，支持单文件处理和批量处理模式。

#### image.c/h
封装图像加载和保存功能：
- `load_image`: 使用stb_image加载图像文件
- `save_image`: 使用stb_image_write保存处理后的图像

#### filters.c/h
实现基础图像处理滤镜：
- `apply_grayscale`: 将彩色图像转换为灰度图
- `apply_invert`: 反转图像颜色
- `apply_blur`: 应用高斯模糊效果

#### edge.c/h
实现边缘检测算法：
- `apply_edge_detection`: 使用Sobel算子进行边缘检测
- `compute_gradient`: 计算像素梯度
- `non_max_suppression`: 非极大值抑制处理

#### ascii_art.c/h
ASCII字符画生成：
- `image_to_ascii`: 将图像转换为ASCII字符画
- `image_to_ascii_styled`: 支持多种风格的ASCII转换

#### rotate.c/h
图像旋转功能：
- `rotate_image`: 使用矩阵变换旋转图像

#### batch.c/h
批量处理功能：
- `batch_process`: 处理指定目录中的所有图像
- `create_output_dirs`: 创建批处理输出目录结构

### 开发环境配置

#### Windows (MinGW)
1. 安装MinGW并添加到PATH环境变量
2. 确保安装了gcc、make和相关工具链
3. 所需依赖:
   - gcc (建议7.0+)
   - make
   - Windows SDK (如果使用MSVC)

#### Linux
1. 安装基础开发工具:
   ```bash
   sudo apt-get install build-essential
   ```
2. 安装开发依赖:
   ```bash
   sudo apt-get install libpng-dev libjpeg-dev
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
程序会自动对输入图像应用五种处理效果，并分别保存为以下文件：
- `grayscale_output.jpg` - 灰度效果
- `blur_output.jpg` - 模糊效果（高斯模糊）
- `invert_output.jpg` - 反色效果
- `rotate_output.jpg` - 旋转效果
- `edge_output.jpg` - 边缘检测效果（增强型Sobel算子，边缘为白色，背景为黑色）

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
- `batch_output/edge/` - 边缘检测结果
- `batch_output/ascii/` - ASCII字符画文件

每个图像会被处理并保存为对应的输出文件，文件名格式为 `原文件名_处理类型.扩展名`。

**使用步骤：**
1. 创建 `batch_input` 目录（如果不存在）
2. 将要处理的图像文件放入 `batch_input` 目录
3. 执行命令 `bin/ImageProcessor --batch`
4. 查看 `batch_output` 目录中的处理结果

## 高级参数调整

### 边缘检测调整
边缘检测功能采用了增强型Sobel算子算法，包含以下特性：
- **双阈值滞后处理**：使用高低两个阈值，确保边缘连续性
- **非极大值抑制**：减少边缘宽度，提高精确度
- **梯度幅值计算**：结合水平和垂直梯度

如需调整边缘检测效果，可修改源代码中的阈值参数：
- 在 `src/main.c` 和 `src/batch.c` 中查找 `edge_threshold` 变量
- 降低阈值（如 30-40）会检测到更多边缘，但可能包含噪声
- 提高阈值（如 60-70）会只保留明显的边缘，减少细节

### ASCII字符画调整
ASCII字符画生成功能提供多种风格和参数：
- **比例因子**：控制输出字符画的大小，数值越大输出越小
- **伽马校正**：调整对比度，值小于1增强暗部细节，大于1增强亮部细节
- **字符集选择**：从简单到复杂的多种字符集

可在 `src/main.c` 中调整以下参数：
```c
// 修改scale_factor(第5个参数)可调整输出大小
// 修改gamma值(最后一个参数)可调整对比度
image_to_ascii_styled(data, width, height, channels, output_file, 5, ASCII_STYLE_BLOCKS, 0.8f);
```

## 安装与配置

### Windows
1. 安装MinGW或MSYS2，确保将编译器路径添加到环境变量中
2. 克隆项目代码：`git clone <项目URL>`
3. 进入项目目录：`cd ImageProcessor`
4. 编译项目：`mingw32-make`
5. 运行程序：`bin\ImageProcessor.exe <input_image>`

### Linux/macOS
1. 确保已安装GCC和Make
2. 克隆项目代码：`git clone <项目URL>`
3. 进入项目目录：`cd ImageProcessor`
4. 编译项目：`make`
5. 运行程序：`./bin/ImageProcessor <input_image>`

## 贡献
欢迎通过以下方式贡献本项目：
1. Fork本仓库
2. 创建功能分支：`git checkout -b feature/amazing-feature`
3. 提交更改：`git commit -m 'Add some amazing feature'`
4. 推送到分支：`git push origin feature/amazing-feature`
5. 提交Pull Request

### 可贡献的方向
- 添加更多图像滤镜和特效
- 改进边缘检测算法
- 优化ASCII字符画生成
- 添加图形用户界面
- 优化性能和内存使用

## 许可证
MIT License