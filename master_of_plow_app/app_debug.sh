cmake -B "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug" -G 'Eclipse CDT4 - Unix Makefiles' -DCMAKE_ECLIPSE_GENERATE_LINKED_RESOURCES=FALSE -DCMAKE_ECLIPSE_VERSION='4.5 (Mars)' -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j$(nproc --ignore=1) -DCMAKE_INSTALL_PREFIX="/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/install" -DCMAKE_PREFIX_PATH="/home/agmo/FeatureDesigner/master_of_plow_fd_v2/.nevonex/dependencies/8.6.0.202606260437;/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/sdk" -DCMAKE_BUILD_TYPE=Debug && cd "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug" && make -j$(nproc --ignore=1) all
cd -
cp "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/sdk/lib/libmaster_of_plow-nevonex.so.8.6.0" "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/src-gen/libmaster_of_plow-nevonex.so"
rm -rf ./sdk/docs
mkdir -p "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/sdk/docs"
cp -r "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/sdk_release/docs/doxygen/*" "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/sdk/docs"
rm -rf "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/docs"
mkdir -p "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/docs"
cd "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/docs"
ln -s "/home/agmo/FeatureDesigner/master_of_plow_fd_v2/master_of_plow/build/app_debug/sdk/docs/html/index.html" master_of_plow_cpp_sdk_doc.html