# Architecture / 架构说明

## 转换流程 / Conversion Flow

```
订阅URL (Subscription URL)
         ↓
    [网络获取 / Network Fetch]
         ↓
    订阅内容 (Raw Subscription)
         ↓
    [解析节点 / Parse Nodes]
    (addNodes函数)
         ↓
    节点列表 (Proxy List)
         ↓
    [预处理 / Preprocess]
    (preprocessNodes函数)
         ↓
    处理后的节点 (Processed Proxies)
         ↓
    [生成Clash配置 / Generate Clash Config]
    (proxyToClash函数)
         ↓
    Clash YAML配置 (Clash YAML Config)
```

## 函数调用关系 / Function Call Graph

```
convertSubscriptionToClash()
  └── convertSubscriptionToClashWithBase()
        ├── addNodes()              // 从URL获取并解析节点
        │     └── explode()         // 解析各种协议的节点
        │           ├── explodeSS()
        │           ├── explodeSSR()
        │           ├── explodeVmess()
        │           └── ...
        │
        ├── preprocessNodes()       // 预处理节点
        │     ├── 应用重命名规则
        │     ├── 添加emoji
        │     └── 排序等
        │
        └── proxyToClash()          // 生成Clash配置
              ├── 生成proxies部分
              ├── 生成proxy-groups
              └── 合并base配置
```

## 文件结构 / File Structure

```
subconverter/
├── src/
│   ├── converter/                          ← 新增：独立转换器
│   │   ├── standalone_converter.h         ← API头文件
│   │   ├── standalone_converter.cpp       ← 实现代码
│   │   └── README.md                      ← 模块文档
│   │
│   ├── example_converter.cpp              ← 新增：示例程序
│   ├── test_standalone_converter.cpp      ← 新增：测试程序
│   │
│   ├── handler/                           ← 原有：处理器
│   │   ├── interfaces.cpp                 (提取源代码)
│   │   └── ...
│   │
│   ├── generator/                         ← 原有：生成器
│   │   └── config/
│   │       ├── subexport.cpp             (使用proxyToClash)
│   │       └── nodemanip.cpp             (使用addNodes)
│   │
│   └── parser/                            ← 原有：解析器
│       └── subparser.cpp                  (使用explode*)
│
├── CMakeLists.txt                         ← 修改：添加新目标
├── README.md                              ← 修改：添加公告
├── README-cn.md                           ← 修改：添加公告
│
├── STANDALONE_CONVERTER.md                ← 新增：中文完整文档
├── STANDALONE_CONVERTER_EN.md             ← 新增：英文完整文档
└── IMPLEMENTATION_SUMMARY.md              ← 新增：实现总结
```

## 依赖关系 / Dependencies

```
standalone_converter
    ├── yaml-cpp          (YAML生成)
    ├── libcurl           (网络请求)
    ├── PCRE2             (正则表达式)
    ├── QuickJS           (脚本支持)
    ├── RapidJSON         (JSON解析)
    └── 原项目的核心模块:
          ├── parser/subparser      (节点解析)
          ├── generator/nodemanip   (节点操作)
          └── generator/subexport   (配置生成)
```

## 对比：原项目 vs 独立函数 / Comparison: Original vs Standalone

### 原项目 (Original Project)
```
启动Web服务器 → 接收HTTP请求 → 解析参数 → 转换 → 返回配置
```
- ✅ 功能完整
- ✅ Web界面
- ✅ 多种格式支持
- ❌ 需要运行服务器
- ❌ 配置复杂

### 独立函数 (Standalone Function)
```
直接调用函数 → 传入URL → 返回配置
```
- ✅ 使用简单
- ✅ 直接集成
- ✅ 无需服务器
- ✅ 专注订阅转Clash
- ❌ 功能相对精简

## 使用场景 / Use Cases

### 适合使用独立函数 / Good for Standalone
- 集成到自己的C++应用程序
- 命令行工具
- 自动化脚本
- 桌面应用程序
- 需要离线处理的场景

### 适合使用原项目 / Good for Original
- 部署公共订阅转换服务
- 需要Web界面
- 支持多种输出格式
- 需要规则管理
- 在线服务

## API示例对比 / API Comparison

### 使用原项目 (Using Original)
```bash
# 需要先启动服务
./subconverter

# 然后通过HTTP访问
curl "http://localhost:25500/sub?target=clash&url=https://..."
```

### 使用独立函数 (Using Standalone)
```cpp
// 直接在代码中调用
#include "converter/standalone_converter.h"

std::string error;
std::string config = convertSubscriptionToClash(url, error);
```

## 性能对比 / Performance

| 指标 | 原项目 | 独立函数 |
|------|--------|----------|
| 启动时间 | 需要启动服务器 | 立即可用 |
| 内存占用 | 较高(Web服务器) | 较低 |
| 调用延迟 | HTTP请求开销 | 直接函数调用 |
| 网络依赖 | 获取订阅+本地HTTP | 仅获取订阅 |

---

## 快速开始 / Quick Start

```cpp
// 1. 包含头文件
#include "converter/standalone_converter.h"

// 2. 调用函数
std::string error;
std::string clash_config = convertSubscriptionToClash(
    "https://justmysocks.net/members/getsub.php?service=xxx&id=xxx",
    error
);

// 3. 检查结果
if (clash_config.empty()) {
    std::cerr << "Error: " << error << std::endl;
} else {
    std::cout << clash_config << std::endl;
}
```

就这么简单！ / That's it!
