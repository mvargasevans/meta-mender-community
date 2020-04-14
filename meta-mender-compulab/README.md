# Mender integration for Compulab boards

Supported boards:

 - CL-SOM-iMX8 - NXP i.MX8 System-on-Module (https://www.compulab.com/products/computer-on-modules/cl-som-imx8-nxp-i-mx-8-system-on-module-computer/)

## Build

See specifics for this board at [Mender Hub](https://hub.mender.io/t/compulab-cl-som-imx8/416).

## Fixed version for warrior branch - local.conf example

```
MACHINE ??= 'cl-som-imx7'
DISTRO ?= 'fsl-imx-fb'
PACKAGE_CLASSES = 'package_deb package_tar'
EXTRA_IMAGE_FEATURES ?= "debug-tweaks"
USER_CLASSES ?= "buildstats image-mklibs image-prelink"
PATCHRESOLVE = "noop"
BB_DISKMON_DIRS ??= "\
    STOPTASKS,${TMPDIR},1G,100K \
    STOPTASKS,${DL_DIR},1G,100K \
    STOPTASKS,${SSTATE_DIR},1G,100K \
    STOPTASKS,/tmp,100M,100K \
    ABORT,${TMPDIR},100M,1K \
    ABORT,${DL_DIR},100M,1K \
    ABORT,${SSTATE_DIR},100M,1K \
    ABORT,/tmp,10M,1K"
PACKAGECONFIG_append_pn-qemu-system-native = " sdl"
PACKAGECONFIG_append_pn-nativesdk-qemu = " sdl"
CONF_VERSION = "1"

DL_DIR ?= "${BSPDIR}/downloads/"
ACCEPT_FSL_EULA = "1"
CORE_IMAGE_EXTRA_INSTALL += " dhcp-client rng-tools cl-uboot cl-deploy cl-camera cl-stest memtester htop iotop tmux iperf3 stress-ng  bt-start video-mode  u-boot-fw-utils  kernel-modules firmware-imx-sdma linux-firmware-wl18xx linux-firmware-wlcommon linux-firmware-ti-connectivity-license bluez5 wpa-supplicant wvdial wvdial-compulab"
IMAGE_FEATURES += " package-management ssh-server-openssh "
LICENSE_FLAGS_WHITELIST = "commercial"
DISTRO_FEATURES_append = " polkit"
#PREFERRED_RPROVIDER_u-boot-fw-utils = "u-boot-fw-utils"
# Make it use for core-images only
# These package cause a root image build conflict with gui images
# CORE_IMAGE_EXTRA_INSTALL += " modemmanager networkmanager "
# Users' Configurations
#IMAGE_ROOTFS_SIZE = "8376320"
#IMAGE_OVERHEAD_FACTOR = "1.0"

### Mender ###
# Appended fragment from meta-mender-community/meta-mender-compulab/templates
PREFERRED_RPROVIDER_u-boot-fw-utils = "u-boot-fw-utils-mender-auto-provided"
IMAGE_INSTALL_append = " kernel-image kernel-devicetree"
IMAGE_FSTYPES_remove = "sdcard sdcard.bz2 mender.bmap tar.bz2 ext4"
MENDER_STORAGE_DEVICE_cl-som-imx8 = "/dev/mmcblk0"
MENDER_STORAGE_DEVICE_cl-som-imx7 = "/dev/mmcblk2"
MENDER_UBOOT_STORAGE_DEVICE_cl-som-imx7 = "1"
MENDER_BOOT_PART_SIZE_MB = "40"
IMAGE_BOOTLOADER_BOOTSECTOR_OFFSET_cl-som-imx8 = "66"
IMAGE_BOOTLOADER_FILE_cl-som-imx8 = "imx-boot-${MACHINE}-${UBOOT_CONFIG}.bin"
do_image_sdimg[depends] += "${@bb.utils.contains('PREFERRED_PROVIDER_virtual/bootloader','u-boot-imx','imx-boot:do_deploy','',d)}"
#KERNEL_DEVICETREE_cl-som-imx7 = "imx7d-sbc-iot-imx7.dtb"
KERNEL_DEVICETREE =  "imx7d-cl-som-imx7.dtb"
#KERNEL_DEVICETREE += "imx7d-sbc-imx7.dtb imx7d-sbc-imx7-m4.dtb imx7d-sbc-imx7-lvds.dtb"
#KERNEL_DEVICETREE += "imx7d-sbc-iot-imx7.dtb imx7d-sbc-iot-imx7-rs485-hdx.dtb imx7d-sbc-iot-imx7-can.dtb"
#KERNEL_DEVICETREE_cl-som-imx7 = "imx7d-sbc-iot-imx7-cl-som-imx7.dtb"
MENDER_DTB_NAME = "imx7d-cl-som-imx7.dtb"
PREFERRED_PROVIDER_u-boot_cl-som-imx8 = "u-boot-imx"
PREFERRED_PROVIDER_u-boot_cl-som-imx7 = "u-boot-compulab"
MENDER_STORAGE_TOTAL_SIZE_MB = "8000"
MENDER_FEATURES_ENABLE_append = " mender-uboot mender-image-sd"
MENDER_FEATURES_ENABLE_append = " mender-uboot"
MENDER_FEATURES_DISABLE_append = " mender-grub mender-image-uefi"

# Appended fragment from meta-mender-community/templates

# This really saves a lot of disk space!
INHERIT += "rm_work"

# The name of the disk image and Artifact that will be built.
# This is what the device will report that it is running, and different updates
# must have different names because Mender will skip installation of an Artifact
# if it is already installed.
MENDER_ARTIFACT_NAME = "release-1"

INHERIT += "mender-full"

DISTRO_FEATURES_append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED = "sysvinit"
VIRTUAL-RUNTIME_initscripts = ""

# Build for Hosted Mender
#
# To get your tenant token:
#    - log in to https://hosted.mender.io
#    - click your email at the top right and then "My organization"
#    - press the "COPY TO CLIPBOARD"
#    - assign content of clipboard to MENDER_TENANT_TOKEN
#
MENDER_SERVER_URL = "https://myserver.io"
#MENDER_TENANT_TOKEN = ""

# Build for Mender demo server
#
# https://docs.mender.io/getting-started/create-a-test-environment
#
# Uncomment below and update IP address to match the machine running the
# Mender demo server
#MENDER_DEMO_HOST_IP_ADDRESS = "192.168.0.100"

# Build for Mender production setup (on-prem)
#
# https://docs.mender.io/artifacts/building-for-production
#
# Uncomment below and update the URL to match your configured domain
# name and provide the path to the generated server.crt file.
#
# NOTE! It is recommend that you provide below information in your custom
# Yocto layer and this is only for demo purposes. See linked documentation
# for additional information.
#MENDER_SERVER_URL = "https://docker.mender.io"
#FILESEXTRAPATHS_prepend_pn-mender := "<DIRECTORY-CONTAINING-server.crt>:"
#SRC_URI_append_pn-mender = " file://server.crt"

# Mender storage configuration
#
# More details on these variables is available at
#    https://docs.mender.io/devices/yocto-project/partition-configuration#configuring-storage
#
# Also, please be sure to check other config files as other
# layers, config fragments, etc may attempt to set values
# for specific platforms.  Using "bitbake -e <image-name>"
# can help determine which files are setting these values
# in a given configuration.
#
# MENDER_STORAGE_TOTAL_SIZE_MB = "2048"
# MENDER_BOOT_PART_SIZE_MB = "16"
# MENDER_DATA_PART_SIZE_MB = "1024"
# MENDER_STORAGE_DEVICE = "/dev/mmcblk0"
# MENDER_BOOT_PART = "${MENDER_STORAGE_DEVICE_BASE}1"
# MENDER_DATA_PART = "${MENDER_STORAGE_DEVICE_BASE}4"
# MENDER_ROOTFS_PART_A = "${MENDER_STORAGE_DEVICE_BASE}2"
# MENDER_ROOTFS_PART_B = "${MENDER_STORAGE_DEVICE_BASE}3"
```
