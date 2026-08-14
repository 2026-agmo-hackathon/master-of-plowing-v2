/*
 * AppMain/CloudDownloadListener.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef APPMAIN_CLOUDDOWNLOADLISTENER_HPP
#define APPMAIN_CLOUDDOWNLOADLISTENER_HPP

#include <ecorecpp/mapping_forward.hpp>
#include <ecore/EObject.hpp>

#include <AppMain_forward.hpp>

#include <nevonex/application/cloud/AbstractCloudDownloadListener.hpp>

/*PROTECTED REGION ID(CloudDownloadListener_pre) ENABLED START*/
// NOTE: the generator that produced this file (8.6.0.202606260437) no longer
// emits a file-scope protected region here, so this block is NOT preserved by
// regeneration — it was silently dropped once already, which broke the build.
// Re-check it after every UPDATE_SDK_APP run.
/*PROTECTED REGION END*/

namespace AppMain
{

    class CloudDownloadListener: public virtual ::ecore::EObject,
            public ::nevonex::app::cloud::AbstractCloudDownloadListener
    {
        /*PROTECTED REGION ID(CloudDownloadListener_commonSection) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

    public:
        CloudDownloadListener();

        virtual ~CloudDownloadListener();

        virtual void _initialize() override;

        // Operations from Parent(s)

        // Operations

        // Attributes

        // References
        /**
         * \brief 
         */
    public:
        virtual ::AppMain::MainController_ptr getMainController() const;
        /**
         * \brief 
         */
    public:
        virtual void setMainController(
                ::AppMain::MainController_ptr _mainController);

    public:

        /*PROTECTED REGION ID(CloudDownloadListener) ENABLED START*/
        // setRddfStagingManager() 는 여기 있었습니다. RDDF는 이제 도착 즉시
        // 적재되므로(handleFile) staging 승인 흐름이 필요하지 않습니다.
        //
        // setRddfStagingManager() used to live here. RDDF files are now loaded on
        // arrival, so there is no staging/approval step to inject a manager for.
        /*PROTECTED REGION END*/

    protected:
        virtual void handleMessage(const std::string &_content) override;
        virtual void handleFile(const ::nevonex::resource::FilePath &_filePath) override;
        CloudDownloadListener_ptr _this()
        {
            return CloudDownloadListener_ptr(this);
        }

    private:
        // Attributes

        // References

        ::AppMain::MainController_ptr m_mainController;

        void broadcastCloudResponseToUi(const std::string &_content);

        /*PROTECTED REGION ID(CloudDownloadListener_privateSection) ENABLED START*/
        // m_rddfStagingManager, handleMapListResponse(), pushMessage() 가 여기
        // 있었습니다. 외부 서버 맵 리스트와 UI 중계가 사라져 모두 불필요합니다.
        //
        // m_rddfStagingManager, handleMapListResponse() and pushMessage() lived
        // here; the external map list and the UI relay they served are both gone.
        /*PROTECTED REGION END*/
    };

} // AppMain

#endif // APPMAIN_CLOUDDOWNLOADLISTENER_HPP
