# Sailfish OS RPM Spec for libmpw

This RPM spec file is for compiling `libmpw` and `libmpw-devel` for
Sailfish OS.

## Build

    cd libmpw
    # Use GitLab because the website is dead
    curl -o mpw-2.6-cli-5-0-g344771db.tar.gz https://gitlab.com/MasterPassword/MasterPassword/-/raw/2f96072e3e42887dfe923924451fe73d77db5d0e/mpw-2.6-cli-5-0-g344771db.tar.gz
    mb2 -t SailfishOS-latest-$ARCH build

If the build finishes successfully you'll now have `libmpw` and `libmpw-devel`
packages in the `RPMS` directory.
