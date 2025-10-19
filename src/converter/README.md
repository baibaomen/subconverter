# Standalone Subscription Converter

这个模块提供了一个独立的函数，用于将订阅链接（包括JustMySocks订阅链接）转换为Clash支持的格式。

## 功能特点

- **独立函数**: 提取了核心转换逻辑，可以独立调用
- **简单易用**: 只需输入订阅链接，即可返回转换后的Clash配置
- **支持多种订阅**: 支持SS、SSR、VMess、Trojan等多种协议
- **适用于JustMySocks**: 完全支持JustMySocks订阅链接

## API 接口

### convertSubscriptionToClash

将订阅URL转换为Clash YAML格式的配置。

```cpp
std::string convertSubscriptionToClash(
    const std::string &subscription_url,
    std::string &error_message
);
```

**参数:**
- `subscription_url`: 订阅链接URL（例如JustMySocks订阅链接）
- `error_message`: 输出参数，如果转换失败会包含错误信息

**返回值:**
- 成功: 返回Clash YAML配置字符串
- 失败: 返回空字符串，错误信息存储在error_message中

### convertSubscriptionToClashWithBase

使用自定义基础配置模板转换订阅URL。

```cpp
std::string convertSubscriptionToClashWithBase(
    const std::string &subscription_url,
    const std::string &base_config_path,
    std::string &error_message
);
```

**参数:**
- `subscription_url`: 订阅链接URL
- `base_config_path`: 自定义Clash基础配置模板路径（可选，留空使用默认配置）
- `error_message`: 输出参数，如果转换失败会包含错误信息

**返回值:**
- 成功: 返回Clash YAML配置字符串
- 失败: 返回空字符串，错误信息存储在error_message中

## 使用示例

### C++ 代码示例

```cpp
#include "converter/standalone_converter.h"
#include <iostream>

int main()
{
    // JustMySocks订阅链接示例
    std::string subscription_url = "https://justmysocks.net/members/getsub.php?service=xxxxx&id=xxxxx";
    
    std::string error;
    std::string clash_config = convertSubscriptionToClash(subscription_url, error);
    
    if (clash_config.empty())
    {
        std::cerr << "转换失败: " << error << std::endl;
        return 1;
    }
    
    // 成功，输出或保存配置
    std::cout << clash_config << std::endl;
    
    return 0;
}
```

### 使用示例程序

项目包含了一个完整的示例程序 `example_converter.cpp`，可以直接使用：

```bash
# 编译项目后运行示例程序
./example_converter "https://justmysocks.net/members/getsub.php?service=xxx&id=xxx"

# 或者保存到文件
./example_converter "https://your-subscription-url" output.yaml
```

## 编译

在CMakeLists.txt中添加以下内容来编译示例程序：

```cmake
# 添加standalone converter库
add_library(standalone_converter 
    src/converter/standalone_converter.cpp
)

# 添加示例程序
add_executable(example_converter
    src/example_converter.cpp
)

# 链接依赖
target_link_libraries(example_converter
    standalone_converter
    # 其他必要的库...
)
```

## 支持的订阅格式

- Shadowsocks (SS)
- ShadowsocksR (SSR)
- V2Ray/VMess
- Trojan
- Snell
- Hysteria
- 以及其他常见的代理协议

## 注意事项

1. 确保网络连接正常，函数需要访问订阅URL获取数据
2. 订阅URL必须是有效的格式
3. 返回的配置是标准的Clash YAML格式
4. 默认配置包含基本的DNS和规则设置，可以通过自定义base配置进行调整

## 错误处理

函数会返回详细的错误信息，常见错误包括：

- 无法访问订阅URL
- 订阅内容解析失败
- 订阅中没有有效的节点
- YAML生成错误

建议始终检查返回值和error_message以获取详细的错误信息。
