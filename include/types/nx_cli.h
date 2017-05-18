/** @file nx_cli.h
 *  @brief Provides necessary types needed for managing Custom CLIs using NXSDK
 *
 *  @author nxos-sdk@cisco.com
 *  @bug No known bugs.
 */

#ifndef __NX_CLI_TYPES_H__
#define __NX_CLI_TYPES_H__

namespace nxos {

/*!
 * @brief Supported Input Parameter Types in a CLI.
 *
 * Types help/guide the user to input the right information for a CLI
 * thereby validating the input in the CLI parser itself rather than 
 * validating the CLI config in the backend callback 
 *
 * Each Type has some optional additional attributes to facilitate
 * the CLI input to be taken for a parameter. Refer to updateParam. 
 *
 *   Type        |  Expected Input Type Attributes data 
 *   --------------------------------------------------    
 *   P_INTEGER   | cli_param_type_integer_attr
 *   P_STRING    | cli_param_type_string_attr
 *   P_INTERFACE | NULL
 *   P_IP_ADDR   | cli_param_type_ip_addr_attr
 *   P_MAC_ADDR  | NULL
 **/
typedef enum {
   /// CLI Param Start Type 
   P_START_TYPE = 0,

   /// CLI Param Type is String
   P_STRING    = 0,

   /// CLI Param Type is Integer
   P_INTEGER   = 1,

   /// CLI Param Type is Interface
   P_INTERFACE = 2,

   /// CLI Param Type is IP address
   P_IP_ADDR,

   /// CLI Param Type is MAC address
   P_MAC_ADDR,

   /// CLI Param Type MAX_TYPE
   P_MAX_TYPE
} param_type_e;

// Supported Record types
typedef enum {
   /// Record Type is TEXT
   R_TEXT = 0,
 
   /// Record Type is JSON   
   R_JSON,

   /// Record Type is XML
   R_XML,

   /// Record Type is MAX_TYPE
   R_MAX_TYPE
} record_type_e;

/**
 * @brief Specifies that the type of the CLI input parameter is an integer.
 *
 * Matching param type is P_INTEGER.
 * When used in 
 *  updateParam context: 
 *    @param[in] min_val minimum allowed value for the input interger parameter.
 *    @param[in] max_val Maximum allowed value for the input interger parameter.
 *
 *  <b> Usage: </b>
 *     If you need to create a CLI which will take an integer between <1-100> <br>
 *      
 *        CLI: $appname port-bw threshold ?
 *             <1-100>  Threshold Limit. Default 50%
 *  
 *  @code{.cpp}
 *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
 *       cliP = sdk->getCliParser();
 *       nxos::cli_param_type_integer_attr int_attr = {0};
 *       int_attr.min_val = 1;
 *       int_attr.max_val = 100;
 *       cmd = cliP->newConfigCmd("set_port_bw_threshold_cmd",
 *                                "port-bw threshold <threshold>");
 *       cmd->updateParam("<threshold>", "Threshold Limit. Default 50%", nxos::P_INTEGER,
 *                        (void *)&int_attr, sizeof(int_attr));
 *  @endcode
 *
 *  @code{.py}
 *       import nx_sdk_py
 *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
 *       cliP = sdk.getCliParser()
 *       int_attr = nx_sdk_py.cli_param_type_integer_attr()
 *       int_attr.min_val = 1;
 *       int_attr.max_val = 100;
 *       cmd = cliP.newConfigCmd("set_port_bw_threshold_cmd", 
 *                               "port-bw threshold <threshold>")
 *       cmd.updateParam("<threshold>", "Threshold Limit. Default 50%", nxos::P_INTEGER,
 *                       int_attr, len(int_attr))
 *  @endcode 
 **/
typedef struct cli_param_type_integer_attr_ {
   int  min_val;  /// Default is 0. 
   int  max_val;  /// Default is 214748364.  
                  /// Default will be set if max_val < min_val 
} cli_param_type_integer_attr;

/**
 * @brief Specifies that the type of the CLI input parameter is string.
 *
 * Matching param type is P_STRING.
 * When used in 
 *  updateParam context: 
 *    @param[in] length  Length of the imput parameter string.
 *    @param[in] regex_pattern Regular expression for the input parameter
 *
 *  Usage:
 *     If you need to create a CLI which will take a string with max length 25
 *     and matches a regex pattern "^[a-zA-Z]+$"
 *  
 *     CLI: $appname encryption-text ?
 *          WORD          Enter Encrypted text (Max Size 25)  
 *
 *  @code{.cpp}
 *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
 *       cliP = sdk->getCliParser();
 *       nxos::cli_param_type_string_attr str_attr = {0};
 *       str_attr.length = 25;
 *       strcpy(str_attr.regex_pattern, pattern);
 *       cmd = cliP->newConfigCmd("set_encryption_text_cmd",
 *                                "encryption-text <text>");
 *       cmd->updateParam("<text>", "Enter Encrypted text", nxos::P_STRING,
 *                        (void *)&str_attr, sizeof(str_attr));
 *  @endcode
 *
 *  @code{.py}
 *       import nx_sdk_py
 *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
 *       cliP = sdk.getCliParser()
 *       str_attr = nx_sdk_py.cli_param_type_string_attr()
 *       str_attr.length = 25;
 *       str_attr.regex_pattern = "^[a-zA-Z]+$";
 *       cmd = cliP.newConfigCmd("set_encryption_text_cmd", 
 *                               "encryption-text <text>")
 *       cmd.updateParam("<text>", "Encrypted text", nxos::P_STRING,
 *                       str_attr, len(str_attr))
 *  @endcode 
 **/
typedef struct cli_param_type_string_attr_ {
   int  length;              /// Default is 256
   char regex_pattern[256];  /// Default is no regex pattern
} cli_param_type_string_attr;

/**
 * @brief Specifies that the type of the CLI input parameter is ipv4 or ipv6 address or prefix.
 *
 * Matching param type is P_IP_ADDR.
 * When used in 
 *  updateParam context: 
 *    @param[in] is_ipv6  If set then Input parameter is a IPv6 addr else 
 *                        its IPv4 address.
 *    @param[in] is_mask  If set then Input parameter is a Prefix.
 *
 *  Usage:
 *     If you need to create a CLI which will take a IP or IPv6 Prefix
 *
 *         CLI: $appname filter-prefix ?
 *              A.B.C.D/LEN        Enter Filter IP Prefix
 *              A:B::C:D/LEN       Enter Filter IPv6 Prefix
 * 
 *  @code{.cpp}
 *       sdk = nxos::NxSdk::getSdkInst(argc, argv);
 *       cliP = sdk->getCliParser();
 *       nxos::cli_param_type_ip_addr_attr ip_pfx_attr = {0};
 *       ip_pfx_attr.is_ipv6 = false;
 *       ip_pfx_attr.is_prefix = true;
 *       cmd = cliP->newConfigCmd("set_filter_prefix_cmd",
 *                                "filter-prefix [<ip-prefix> | <ipv6-prefix>]");
 *       cmd->updateParam("<ip-prefix>", "Enter Filter IP Prefix", nxos::P_IP_ADDR,
 *                        (void *)&ip_pfx_attr, sizeof(ip_pfx_attr));
 *       ip_pfx_attr.is_ipv6 = true;
 *       cmd->updateParam("<ipv6-prefix>", "Enter Filter IPv6 Prefix", nxos::P_IP_ADDR,
 *                        (void *)&ip_pfx_attr, sizeof(ip_pfx_attr));
 *
 *  @endcode
 *
 *  @code{.py}
 *       import nx_sdk_py
 *       sdk = nx_sdk_py.NxSdk.getSdkInst(len(sys.argv), sys.argv)
 *       cliP = sdk.getCliParser()
 *       ip_pfx_attr = nx_sdk_py.cli_param_type_ip_addr_attr()
 *       ip_pfx_attr.is_ipv6 = False
 *       ip_pfx_attr.is_prefix = True
 *       cmd = cliP.newConfigCmd("set_filter_prefix_cmd",
 *                               "filter-prefix [<ip-prefix> | <ipv6-prefix>]")
 *       cmd.updateParam("<ip-prefix>", "Enter Filter IP Prefix", nxos::P_IP_ADDR,
 *                       ip_pfx_attr, len(ip_pfx_attr))
 *       ip_pfx_attr.is_ipv6 = True
 *       cmd.updateParam("<ipv6-prefix>", "Enter Filter IPv6 Prefix", nxos::P_IP_ADDR,
 *                       ip_pfx_attr, len(ip_pfx_attr));
 *  @endcode 
 *
 **/
typedef struct cli_param_type_ip_addr_attr_ {
   bool  is_ipv6;    /// Default is false
   bool  is_prefix;  /// Default is false  
} cli_param_type_ip_addr_attr;

}

#endif //__NX_TYPES__

