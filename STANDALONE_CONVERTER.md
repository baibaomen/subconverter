# 独立订阅转换函数 (Standalone Subscription Converter)

## 概述

本项目新增了一个独立的函数，可以将任何订阅链接（包括JustMySocks订阅链接）转换为Clash支持的YAML格式。这个功能提取了subconverter的核心转换逻辑，并将其暴露为一个简单易用的API函数。

## 主要特性

- ✅ **独立函数**: 无需启动完整的Web服务器，可以直接调用
- ✅ **简单API**: 仅需输入订阅URL，即可返回Clash配置
- ✅ **支持多种协议**: SS、SSR、VMess、Trojan、Hysteria等
- ✅ **完全支持JustMySocks**: 专门优化支持JustMySocks订阅格式
- ✅ **错误处理**: 完善的错误处理和错误信息返回
- ✅ **自定义配置**: 支持使用自定义的Clash基础配置模板

## 快速开始

### 基本用法

```cpp
#include "converter/standalone_converter.h"
#include <iostream>

int main()
{
    // JustMySocks订阅链接
    std::string url = "https://justmysocks.net/members/getsub.php?service=xxxxx&id=xxxxx";
    
    std::string error;
    std::string clash_config = convertSubscriptionToClash(url, error);
    
    if (clash_config.empty()) {
        std::cerr << "转换失败: " << error << std::endl;
        return 1;
    }
    
    std::cout << clash_config << std::endl;
    return 0;
}
```

### 使用自定义基础配置

```cpp
std::string error;
std::string clash_config = convertSubscriptionToClashWithBase(
    subscription_url,
    "path/to/custom/base.yaml",  // 自定义base配置路径
    error
);
```

## API 文档

### convertSubscriptionToClash

将订阅URL转换为Clash YAML配置。

**函数签名:**
```cpp
std::string convertSubscriptionToClash(
    const std::string &subscription_url,
    std::string &error_message
);
```

**参数:**
- `subscription_url`: 订阅链接URL（例如JustMySocks订阅链接）
- `error_message`: [输出参数] 如果转换失败，会包含详细的错误信息

**返回值:**
- 成功: 返回Clash YAML格式的配置字符串
- 失败: 返回空字符串，错误信息存储在`error_message`中

### convertSubscriptionToClashWithBase

使用自定义基础配置模板转换订阅URL。

**函数签名:**
```cpp
std::string convertSubscriptionToClashWithBase(
    const std::string &subscription_url,
    const std::string &base_config_path,
    std::string &error_message
);
```

**参数:**
- `subscription_url`: 订阅链接URL
- `base_config_path`: 自定义Clash基础配置模板路径（留空使用默认配置）
- `error_message`: [输出参数] 如果转换失败，会包含详细的错误信息

**返回值:**
- 成功: 返回Clash YAML格式的配置字符串
- 失败: 返回空字符串

## 编译和安装

### 前提条件

项目依赖以下库：
- C++20编译器
- CMake 3.5+
- yaml-cpp >= 0.6.3
- PCRE2
- QuickJS
- libcurl >= 7.54.0
- RapidJSON
- toml11
- LibCron

### 编译步骤

```bash
# 1. 克隆仓库
git clone https://github.com/baibaomen/subconverter.git
cd subconverter

# 2. 创建build目录
mkdir build && cd build

# 3. 配置和编译
cmake ..
make

# 4. 运行示例程序
./example_converter "https://your-subscription-url"
```

### 集成到你的项目

在你的项目的CMakeLists.txt中：

```cmake
# 添加standalone_converter库
add_subdirectory(path/to/subconverter)

# 链接到你的目标
target_link_libraries(your_target standalone_converter)
```

## 示例程序

项目包含了一个完整的示例程序 `example_converter.cpp`，演示如何使用转换函数：

```bash
# 转换JustMySocks订阅并输出到stdout
./example_converter "https://justmysocks.net/members/getsub.php?service=xxx&id=xxx"

# 转换订阅并保存到文件
./example_converter "https://your-subscription-url" output.yaml
```

**示例输出:**
```
Converting subscription URL to Clash format...
URL: https://justmysocks.net/members/getsub.php?service=xxx&id=xxx

Conversion successful!
Generated configuration length: 2567 bytes

=== Generated Clash Configuration ===
port: 7890
socks-port: 7891
allow-lan: false
mode: Rule
log-level: info
...
proxies:
  - name: "HK-1"
    type: ss
    server: xxx.xxx.xxx.xxx
    port: 8388
    ...
```

## 支持的订阅格式

- ✅ Shadowsocks (SS / SIP002 / SIP008)
- ✅ ShadowsocksR (SSR)
- ✅ V2Ray/VMess
- ✅ Trojan
- ✅ Snell
- ✅ Hysteria / Hysteria2
- ✅ SOCKS / HTTP代理
- ✅ SSD格式
- ✅ Base64编码的订阅

## JustMySocks 特别说明

该函数完全支持JustMySocks的订阅格式。JustMySocks的订阅链接格式通常为：

```
https://justmysocks.net/members/getsub.php?service=xxxxx&id=xxxxx
```

转换后会生成包含所有节点的完整Clash配置，包括：
- 所有可用的代理节点
- 自动测速选择组
- 基础的DNS配置
- 默认的路由规则

## 错误处理

函数提供详细的错误信息，常见错误包括：

| 错误类型 | 描述 | 解决方法 |
|---------|------|---------|
| 网络错误 | 无法访问订阅URL | 检查网络连接和URL有效性 |
| 解析错误 | 订阅内容格式不正确 | 确认订阅链接是否有效 |
| 空订阅 | 订阅中没有找到任何节点 | 检查订阅是否过期或被清空 |
| YAML错误 | YAML生成失败 | 检查节点数据是否完整 |

**错误处理示例:**
```cpp
std::string error;
std::string config = convertSubscriptionToClash(url, error);

if (config.empty()) {
    if (error.find("Failed to fetch") != std::string::npos) {
        std::cerr << "网络问题: " << error << std::endl;
    } else if (error.find("No nodes") != std::string::npos) {
        std::cerr << "订阅为空: " << error << std::endl;
    } else {
        std::cerr << "其他错误: " << error << std::endl;
    }
    return 1;
}
```

## 高级用法

### 自定义节点筛选

默认配置会包含所有解析到的节点。如果需要自定义筛选，可以修改 `standalone_converter.cpp` 中的 `parse_set` 配置：

```cpp
// 在standalone_converter.cpp中设置排除规则
string_array exclude_patterns = {"过期", "剩余", "官网"};
parse_set.exclude_remarks = &exclude_patterns;
```

### 自定义代理组

可以通过修改返回的YAML来自定义代理组：

```cpp
std::string config = convertSubscriptionToClash(url, error);
YAML::Node yaml = YAML::Load(config);

// 添加自定义代理组
YAML::Node custom_group;
custom_group["name"] = "🚀 节点选择";
custom_group["type"] = "select";
// ... 添加更多配置

yaml["proxy-groups"].push_back(custom_group);
std::string modified_config = YAML::Dump(yaml);
```

## 性能考虑

- 函数会通过网络获取订阅内容，耗时取决于网络速度
- 解析和转换过程通常在毫秒级完成
- 对于大型订阅（>100个节点），转换可能需要几百毫秒
- 建议在异步线程中调用以避免阻塞主线程

## 与原项目的关系

这个独立函数基于subconverter项目的核心逻辑，但做了以下简化：
- 移除了Web服务器相关代码
- 简化了配置管理
- 专注于单一功能：订阅到Clash的转换
- 提供了更简洁的API接口

完整的subconverter项目提供了Web服务、规则管理、多格式支持等更多功能，适合部署为公共服务。而这个独立函数适合集成到其他C++项目中使用。

## 许可证

本项目遵循与subconverter相同的许可证（GPL-3.0）。

## 贡献

欢迎提交Issue和Pull Request！

## 相关链接

- [原始subconverter项目](https://github.com/tindy2013/subconverter)
- [Clash文档](https://github.com/Dreamacro/clash/wiki)
- [JustMySocks官网](https://justmysocks.net)

## 常见问题 (FAQ)

### Q: 这个函数需要配置文件吗？
A: 不需要。函数使用内置的默认配置，无需外部配置文件。

### Q: 可以在多线程环境中使用吗？
A: 可以，但建议为每个线程创建独立的调用，避免共享状态。

### Q: 支持离线转换吗？
A: 不支持。函数需要访问网络来获取订阅内容。

### Q: 生成的配置可以直接用于Clash吗？
A: 是的，生成的是标准的Clash YAML配置，可以直接使用。

### Q: 如何处理订阅更新？
A: 定期调用函数重新获取订阅即可，建议设置合适的更新间隔（如每小时或每天）。
