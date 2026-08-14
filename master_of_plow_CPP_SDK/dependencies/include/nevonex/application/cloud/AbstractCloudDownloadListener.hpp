/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SRC_APPLICATION_CLOUD_ABSTRACTCLOUDDOWNLOADLISTENER_HPP_
#define SRC_APPLICATION_CLOUD_ABSTRACTCLOUDDOWNLOADLISTENER_HPP_

#include "../../propertychange/PropertyChangeListener.hpp"
#include <nevonex-fcal-platform/resource/FilePath.hpp>

namespace nevonex
{
namespace app
{
namespace cloud
{

class AbstractCloudDownloadListener : public virtual ::nevonex::propertychange::PropertyChangeListener
{
public:
	AbstractCloudDownloadListener() = default;
	virtual ~AbstractCloudDownloadListener() = default;
protected:
	/**
	 * \brief For Handling Message Content Available.
	 *
	 * \param _content
	 * \return void
	 */
	virtual void handleMessage(const std::string & _content) = 0;
	/**
	 * \brief For Handling File Download Available.
	 *
	 * \param _filePath Path to the file
	 * \return void
	 */
	virtual void handleFile(const ::nevonex::resource::FilePath & _filePath) = 0;

private:
	virtual void propertyChange(::nevonex::propertychange::PropertyChangeEvent<const ::ecore::EJavaObject&, const ::ecore::EJavaObject& > changeEvent) override;
};

} /* namespace cloud */
} /* namespace app */
} /* namespace nevonex */

#endif /* SRC_APPLICATION_CLOUD_ABSTRACTCLOUDDOWNLOADLISTENER_HPP_ */
