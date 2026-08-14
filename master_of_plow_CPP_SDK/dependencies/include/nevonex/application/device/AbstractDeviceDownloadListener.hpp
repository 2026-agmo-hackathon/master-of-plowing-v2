/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SRC_APPLICATION_DEVICE_ABSTRACTDEVICEDOWNLOADLISTENER_HPP_
#define SRC_APPLICATION_DEVICE_ABSTRACTDEVICEDOWNLOADLISTENER_HPP_

#include "../../propertychange/PropertyChangeListener.hpp"
#include <nevonex-fcal-platform/resource/FilePath.hpp>

namespace nevonex
{
namespace app
{
namespace device
{

class AbstractDeviceDownloadListener  : public virtual ::nevonex::propertychange::PropertyChangeListener
{
public:
	AbstractDeviceDownloadListener() = default;
	virtual ~AbstractDeviceDownloadListener() = default;
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

} /* namespace device */
} /* namespace app */
} /* namespace nevonex */

#endif /* SRC_APPLICATION_DEVICE_ABSTRACTDEVICEDOWNLOADLISTENER_HPP_ */
