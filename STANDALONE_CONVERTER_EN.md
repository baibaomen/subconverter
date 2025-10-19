# Standalone Subscription Converter

[中文文档](./STANDALONE_CONVERTER.md)

## Overview

This project introduces a standalone function that converts any subscription URL (including JustMySocks subscription links) to Clash-compatible YAML format. This feature extracts the core conversion logic from subconverter and exposes it as a simple, easy-to-use API function.

## Key Features

- ✅ **Standalone Function**: Direct function call without needing a full web server
- ✅ **Simple API**: Just input subscription URL, get Clash config
- ✅ **Multi-Protocol Support**: SS, SSR, VMess, Trojan, Hysteria, and more
- ✅ **Full JustMySocks Support**: Optimized for JustMySocks subscription format
- ✅ **Error Handling**: Comprehensive error handling with detailed error messages
- ✅ **Custom Configuration**: Support for custom Clash base configuration templates

## Quick Start

### Basic Usage

```cpp
#include "converter/standalone_converter.h"
#include <iostream>

int main()
{
    // JustMySocks subscription link
    std::string url = "https://justmysocks.net/members/getsub.php?service=xxxxx&id=xxxxx";
    
    std::string error;
    std::string clash_config = convertSubscriptionToClash(url, error);
    
    if (clash_config.empty()) {
        std::cerr << "Conversion failed: " << error << std::endl;
        return 1;
    }
    
    std::cout << clash_config << std::endl;
    return 0;
}
```

### Using Custom Base Configuration

```cpp
std::string error;
std::string clash_config = convertSubscriptionToClashWithBase(
    subscription_url,
    "path/to/custom/base.yaml",  // Custom base config path
    error
);
```

## API Documentation

### convertSubscriptionToClash

Converts a subscription URL to Clash YAML configuration.

**Function Signature:**
```cpp
std::string convertSubscriptionToClash(
    const std::string &subscription_url,
    std::string &error_message
);
```

**Parameters:**
- `subscription_url`: Subscription URL (e.g., JustMySocks subscription link)
- `error_message`: [Output parameter] Contains detailed error message if conversion fails

**Return Value:**
- Success: Returns Clash YAML configuration string
- Failure: Returns empty string, error details stored in `error_message`

### convertSubscriptionToClashWithBase

Converts subscription URL using a custom base configuration template.

**Function Signature:**
```cpp
std::string convertSubscriptionToClashWithBase(
    const std::string &subscription_url,
    const std::string &base_config_path,
    std::string &error_message
);
```

**Parameters:**
- `subscription_url`: Subscription URL
- `base_config_path`: Path to custom Clash base configuration template (leave empty for default)
- `error_message`: [Output parameter] Contains error details if conversion fails

**Return Value:**
- Success: Returns Clash YAML configuration string
- Failure: Returns empty string

## Build and Installation

### Prerequisites

The project requires the following libraries:
- C++20 compiler
- CMake 3.5+
- yaml-cpp >= 0.6.3
- PCRE2
- QuickJS
- libcurl >= 7.54.0
- RapidJSON
- toml11
- LibCron

### Build Steps

```bash
# 1. Clone repository
git clone https://github.com/baibaomen/subconverter.git
cd subconverter

# 2. Create build directory
mkdir build && cd build

# 3. Configure and build
cmake ..
make

# 4. Run example program
./example_converter "https://your-subscription-url"
```

### Integrating into Your Project

In your project's CMakeLists.txt:

```cmake
# Add standalone_converter library
add_subdirectory(path/to/subconverter)

# Link to your target
target_link_libraries(your_target standalone_converter)
```

## Example Program

The project includes a complete example program `example_converter.cpp` demonstrating how to use the converter:

```bash
# Convert JustMySocks subscription and output to stdout
./example_converter "https://justmysocks.net/members/getsub.php?service=xxx&id=xxx"

# Convert subscription and save to file
./example_converter "https://your-subscription-url" output.yaml
```

**Example Output:**
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

## Supported Subscription Formats

- ✅ Shadowsocks (SS / SIP002 / SIP008)
- ✅ ShadowsocksR (SSR)
- ✅ V2Ray/VMess
- ✅ Trojan
- ✅ Snell
- ✅ Hysteria / Hysteria2
- ✅ SOCKS / HTTP proxies
- ✅ SSD format
- ✅ Base64-encoded subscriptions

## JustMySocks Notes

The function fully supports JustMySocks subscription format. JustMySocks subscription URLs typically look like:

```
https://justmysocks.net/members/getsub.php?service=xxxxx&id=xxxxx
```

After conversion, it generates a complete Clash configuration including:
- All available proxy nodes
- Auto-selection group with health check
- Basic DNS configuration
- Default routing rules

## Error Handling

The function provides detailed error messages. Common errors include:

| Error Type | Description | Solution |
|-----------|-------------|----------|
| Network Error | Cannot access subscription URL | Check network connection and URL validity |
| Parse Error | Subscription content format incorrect | Verify subscription link is valid |
| Empty Subscription | No nodes found in subscription | Check if subscription is expired or empty |
| YAML Error | YAML generation failed | Check if node data is complete |

**Error Handling Example:**
```cpp
std::string error;
std::string config = convertSubscriptionToClash(url, error);

if (config.empty()) {
    if (error.find("Failed to fetch") != std::string::npos) {
        std::cerr << "Network issue: " << error << std::endl;
    } else if (error.find("No nodes") != std::string::npos) {
        std::cerr << "Empty subscription: " << error << std::endl;
    } else {
        std::cerr << "Other error: " << error << std::endl;
    }
    return 1;
}
```

## Advanced Usage

### Custom Node Filtering

Default configuration includes all parsed nodes. To customize filtering, modify the `parse_set` configuration in `standalone_converter.cpp`:

```cpp
// Set exclusion rules in standalone_converter.cpp
string_array exclude_patterns = {"expired", "remaining", "official"};
parse_set.exclude_remarks = &exclude_patterns;
```

### Custom Proxy Groups

Customize proxy groups by modifying the returned YAML:

```cpp
std::string config = convertSubscriptionToClash(url, error);
YAML::Node yaml = YAML::Load(config);

// Add custom proxy group
YAML::Node custom_group;
custom_group["name"] = "🚀 Proxy Select";
custom_group["type"] = "select";
// ... add more configuration

yaml["proxy-groups"].push_back(custom_group);
std::string modified_config = YAML::Dump(yaml);
```

## Performance Considerations

- Function fetches subscription content over network; time depends on network speed
- Parsing and conversion typically completes in milliseconds
- For large subscriptions (>100 nodes), conversion may take several hundred milliseconds
- Recommend calling in async thread to avoid blocking main thread

## Relationship with Original Project

This standalone function is based on subconverter's core logic but with simplifications:
- Removed web server related code
- Simplified configuration management
- Focused on single functionality: subscription to Clash conversion
- Provides cleaner API interface

The full subconverter project offers web service, rule management, multi-format support, and more features suitable for deployment as a public service. This standalone function is suitable for integration into other C++ projects.

## License

This project follows the same license as subconverter (GPL-3.0).

## Contributing

Issues and Pull Requests are welcome!

## Related Links

- [Original subconverter project](https://github.com/tindy2013/subconverter)
- [Clash documentation](https://github.com/Dreamacro/clash/wiki)
- [JustMySocks official site](https://justmysocks.net)

## FAQ

### Q: Does this function require configuration files?
A: No. The function uses built-in default configuration without requiring external config files.

### Q: Can it be used in multi-threaded environments?
A: Yes, but recommend creating independent calls for each thread to avoid shared state.

### Q: Does it support offline conversion?
A: No. The function needs network access to fetch subscription content.

### Q: Can the generated config be used directly with Clash?
A: Yes, it generates standard Clash YAML configuration ready for use.

### Q: How to handle subscription updates?
A: Periodically call the function to re-fetch the subscription. Recommend setting appropriate update intervals (e.g., hourly or daily).
