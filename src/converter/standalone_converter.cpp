#include "standalone_converter.h"

#include <vector>
#include <yaml-cpp/yaml.h>

#include "config/regmatch.h"
#include "generator/config/nodemanip.h"
#include "generator/config/subexport.h"
#include "parser/config/proxy.h"
#include "utils/file_extra.h"
#include "utils/logger.h"
#include "utils/string.h"

// Default simple Clash base configuration
static const char* DEFAULT_CLASH_BASE = R"(
port: 7890
socks-port: 7891
allow-lan: false
mode: Rule
log-level: info
external-controller: 127.0.0.1:9090
dns:
  enable: true
  listen: 0.0.0.0:53
  enhanced-mode: fake-ip
  nameserver:
    - 119.29.29.29
    - 223.5.5.5
  fallback:
    - 8.8.8.8
    - 1.1.1.1
proxies: []
proxy-groups: []
rules: []
)";

std::string convertSubscriptionToClash(
    const std::string &subscription_url,
    std::string &error_message
)
{
    return convertSubscriptionToClashWithBase(subscription_url, "", error_message);
}

std::string convertSubscriptionToClashWithBase(
    const std::string &subscription_url,
    const std::string &base_config_path,
    std::string &error_message
)
{
    try
    {
        // Initialize variables
        std::vector<Proxy> nodes;
        std::string subInfo;
        std::string proxy; // No proxy for standalone usage
        
        // Set up basic parsing settings
        parse_settings parse_set;
        string_array empty_exclude;
        string_array empty_include;
        RegexMatchConfigs empty_streams;
        RegexMatchConfigs empty_times;
        
        parse_set.proxy = &proxy;
        parse_set.exclude_remarks = &empty_exclude;
        parse_set.include_remarks = &empty_include;
        parse_set.stream_rules = &empty_streams;
        parse_set.time_rules = &empty_times;
        parse_set.sub_info = &subInfo;
        parse_set.authorized = true;
        parse_set.request_header = nullptr;
        parse_set.js_runtime = nullptr;
        parse_set.js_context = nullptr;
        
        // Parse the subscription URL and extract nodes
        writeLog(0, "Fetching node data from url '" + subscription_url + "'.", LOG_LEVEL_INFO);
        int result = addNodes(subscription_url, nodes, 0, parse_set);
        
        if (result == -1)
        {
            error_message = "Failed to fetch or parse subscription URL: " + subscription_url;
            return "";
        }
        
        if (nodes.empty())
        {
            error_message = "No nodes were found in the subscription";
            return "";
        }
        
        writeLog(0, "Successfully parsed " + std::to_string(nodes.size()) + " nodes", LOG_LEVEL_INFO);
        
        // Set up extra settings for Clash generation
        extra_settings ext;
        ext.nodelist = false;
        ext.add_emoji = false;
        ext.remove_emoji = false;
        ext.append_proxy_type = false;
        ext.sort_flag = false;
        ext.filter_deprecated = true;
        ext.clash_new_field_name = true;
        ext.clash_script = false;
        ext.clash_classical_ruleset = false;
        ext.enable_rule_generator = false;
        ext.overwrite_original_rules = false;
        ext.clash_proxies_style = "flow";
        ext.clash_proxy_groups_style = "flow";
        ext.authorized = true;
        
        // Preprocess nodes (apply renames, emoji, etc. if needed)
        preprocessNodes(nodes, ext);
        
        // Load or use default base configuration
        std::string base_content;
        if (!base_config_path.empty())
        {
            base_content = fileGet(base_config_path, false);
            if (base_content.empty())
            {
                writeLog(0, "Failed to load base config from: " + base_config_path + ", using default", LOG_LEVEL_WARNING);
                base_content = DEFAULT_CLASH_BASE;
            }
        }
        else
        {
            base_content = DEFAULT_CLASH_BASE;
        }
        
        // Generate Clash YAML - node list only mode
        YAML::Node yamlnode;
        ProxyGroupConfigs empty_groups;
        proxyToClash(nodes, yamlnode, empty_groups, false, ext);
        
        // Parse base config and merge with generated nodes
        try
        {
            YAML::Node base_yaml = YAML::Load(base_content);
            
            // Copy nodes from generated YAML to base
            if (yamlnode["proxies"])
            {
                base_yaml["proxies"] = yamlnode["proxies"];
            }
            
            // If base doesn't have proxy-groups, create a simple auto group
            if (!base_yaml["proxy-groups"] || base_yaml["proxy-groups"].size() == 0)
            {
                YAML::Node auto_group;
                auto_group["name"] = "auto";
                auto_group["type"] = "url-test";
                auto_group["url"] = "http://www.gstatic.com/generate_204";
                auto_group["interval"] = 300;
                
                YAML::Node proxies_list;
                if (yamlnode["proxies"])
                {
                    for (const auto& proxy : yamlnode["proxies"])
                    {
                        if (proxy["name"])
                        {
                            proxies_list.push_back(proxy["name"].as<std::string>());
                        }
                    }
                }
                auto_group["proxies"] = proxies_list;
                
                base_yaml["proxy-groups"].push_back(auto_group);
            }
            
            // If base doesn't have rules, add a simple rule
            if (!base_yaml["rules"] || base_yaml["rules"].size() == 0)
            {
                YAML::Node rules;
                rules.push_back("MATCH,auto");
                base_yaml["rules"] = rules;
            }
            
            // Convert to YAML string
            YAML::Emitter emitter;
            emitter << base_yaml;
            
            return emitter.c_str();
        }
        catch (const YAML::Exception& e)
        {
            error_message = "YAML parsing error: " + std::string(e.what());
            return "";
        }
    }
    catch (const std::exception& e)
    {
        error_message = "Exception during conversion: " + std::string(e.what());
        return "";
    }
    catch (...)
    {
        error_message = "Unknown exception during conversion";
        return "";
    }
}
