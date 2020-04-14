FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

MENDER_UBOOT_PRE_SETUP_COMMANDS_cl-som-imx7 = "setenv kernel_addr_r \${loadaddr}; setenv bootargs console=\${console},\${baudrate}"
MENDER_UBOOT_PRE_SETUP_COMMANDS_cl-som-imx8 = "setenv kernel_addr_r \${loadaddr}"

SRC_URI_append_cl-som-imx8 += " file://0001-tools-fix-cross-compiling-tools-when-HOSTCC-is-overr.patch"

# Fix for the warrior branch
# 0003 gets overwritten
#SRC_URI_append_mender-uboot = " file://0003-Integration-of-Mender-boot-code-into-U-Boot.patch"

# 0004 gets overwritten
#SRC_URI_append_mender-uboot = "${@bb.utils.contains('MENDER_UBOOT_AUTO_CONFIGURE', \
#                                                    '1', \
#                                                    ' file://0004-Disable-CONFIG_BOOTCOMMAND-and-enable-CONFIG_MENDER_.patch', \
#                                                    '', \
#                                                    d)}"

# 0005 is added according to Mender documentation to troubleshoot u-boot
SRC_URI_append = " file://0005-fw_env_main.c-Fix-incorrect-size-for-malloc-ed-strin.patch"

