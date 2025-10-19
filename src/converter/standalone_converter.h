#ifndef STANDALONE_CONVERTER_H_INCLUDED
#define STANDALONE_CONVERTER_H_INCLUDED

#include <string>

/**
 * @brief Convert a subscription URL (including JustMySocks) to Clash YAML format
 * 
 * This is a standalone function that extracts the core conversion logic
 * from the subconverter project. It takes a subscription URL and converts
 * it to Clash-compatible YAML format.
 * 
 * @param subscription_url The subscription URL to convert (e.g., JustMySocks subscription link)
 * @param error_message Output parameter to store error message if conversion fails
 * @return std::string The converted Clash YAML configuration, or empty string on error
 * 
 * Example usage:
 * @code
 *   std::string error;
 *   std::string clash_config = convertSubscriptionToClash(
 *       "https://justmysocks.net/members/getsub.php?service=xxxx&id=xxxxx",
 *       error
 *   );
 *   if (clash_config.empty()) {
 *       std::cerr << "Conversion failed: " << error << std::endl;
 *   } else {
 *       std::cout << clash_config << std::endl;
 *   }
 * @endcode
 */
std::string convertSubscriptionToClash(
    const std::string &subscription_url,
    std::string &error_message
);

/**
 * @brief Convert a subscription URL to Clash format with custom base configuration
 * 
 * @param subscription_url The subscription URL to convert
 * @param base_config_path Path to custom Clash base configuration template (optional, can be empty)
 * @param error_message Output parameter to store error message if conversion fails
 * @return std::string The converted Clash YAML configuration, or empty string on error
 */
std::string convertSubscriptionToClashWithBase(
    const std::string &subscription_url,
    const std::string &base_config_path,
    std::string &error_message
);

#endif // STANDALONE_CONVERTER_H_INCLUDED
