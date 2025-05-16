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

## 依赖
- C 编译器 (GCC/Clang/Visual Studio)
- `stb_image.h` 和 `stb_image_write.h`（第三方库）

## 构建
1. 安装依赖。
2. 在项目根目录下运行：
```sh
make
```
3. 可执行文件将在 `bin/` 目录下生成。

## 项目结构
```
.
├── src                    // 源代码目录
│   ├── main.c
│   ├── image.c
│   ├── filters.c
│   ├── ascii_art.c
│   ├── edge.c
│   ├── rotate.c
│   └── batch.c
├── include                // 头文件目录
│   ├── image.h
│   ├── filters.h
│   ├── ascii_art.h
│   ├── edge.h
│   ├── rotate.h
│   └── batch.h
├── third_party
│   └── stb
│       ├── stb_image.h
│       └── stb_image_write.h
├── lib                    // 编译后库文件
├── bin                    // 可执行文件输出
├── Makefile               // 构建脚本
└── README.md              // 项目说明
```

## 使用示例
```sh
bin/ImageProcessor input.jpg output.jpg --grayscale
```

## 许可证
MIT License
