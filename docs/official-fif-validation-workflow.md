# Official FIF validation workflow

The remote emulator must remain unmodified. Validation is performed with an
official FIF package, not by copying an executable into an installed feature's
root filesystem.

## Current repository boundary

This participant repository contains the application source, generated model
source, manifests, UI source, and tests needed to develop and verify the
skeleton. It intentionally excludes the proprietary/generated
`master_of_plow_CPP_SDK`, IDE build output, and packaged FIF artifacts.

Consequently, the repository can build and test the C++ executable in a
supported Linux toolchain, but it cannot independently produce a provenance-
complete FIF. `Manifest.xml` alone is not a packager. Do not reconstruct a FIF
with an ad-hoc archive command and do not install a loose executable.

## Required official flow

1. Open the project in the supported NEVONEX/SeamOS IDE environment that owns
   the matching generated C++ SDK and FIF packaging pipeline.
2. Build the application and package the resulting rootfs, manifest, feature
   configuration, interface metadata, SDK shared library, and UI using that
   pipeline.
3. Record the source identifier, packaged FIF SHA-256, application binary
   SHA-256, SDK-library SHA-256, manifest version, and toolchain/IDE version.
4. Install the `.fif` through the emulator's normal FIF upload/install workflow
   and wait for the IoT installation job to complete.
5. Leave execution to the user's normal application-selection/start workflow.
   Do not directly replace files in `app_data`, `repo`, or a running process.
6. Validate against the original emulator UI, simcan signals, and nginx config.

An installed FIF is evidence only for the source identity embedded in that
package. For every release, record the version and hashes again and install the
new official artifact; never infer provenance from an older retained package.
