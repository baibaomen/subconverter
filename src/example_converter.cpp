/**
 * @file example_converter.cpp
 * @brief Example program demonstrating the standalone subscription converter
 * 
 * This example shows how to use the convertSubscriptionToClash function
 * to convert any subscription URL (including JustMySocks) to Clash format.
 */

#include <iostream>
#include <fstream>
#include "converter/standalone_converter.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <subscription_url> [output_file]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Example:" << std::endl;
        std::cerr << "  " << argv[0] << " \"https://justmysocks.net/members/getsub.php?service=xxx&id=xxx\"" << std::endl;
        std::cerr << "  " << argv[0] << " \"https://your-subscription-url\" clash_config.yaml" << std::endl;
        return 1;
    }
    
    std::string subscription_url = argv[1];
    std::string output_file;
    if (argc >= 3)
    {
        output_file = argv[2];
    }
    
    std::cout << "Converting subscription URL to Clash format..." << std::endl;
    std::cout << "URL: " << subscription_url << std::endl;
    std::cout << std::endl;
    
    std::string error_message;
    std::string clash_config = convertSubscriptionToClash(subscription_url, error_message);
    
    if (clash_config.empty())
    {
        std::cerr << "ERROR: Conversion failed!" << std::endl;
        std::cerr << "Error message: " << error_message << std::endl;
        return 1;
    }
    
    std::cout << "Conversion successful!" << std::endl;
    std::cout << "Generated configuration length: " << clash_config.length() << " bytes" << std::endl;
    std::cout << std::endl;
    
    // Output to file if specified, otherwise to stdout
    if (!output_file.empty())
    {
        std::ofstream outfile(output_file);
        if (!outfile)
        {
            std::cerr << "ERROR: Failed to open output file: " << output_file << std::endl;
            return 1;
        }
        
        outfile << clash_config;
        outfile.close();
        
        std::cout << "Configuration saved to: " << output_file << std::endl;
    }
    else
    {
        std::cout << "=== Generated Clash Configuration ===" << std::endl;
        std::cout << clash_config << std::endl;
    }
    
    return 0;
}
