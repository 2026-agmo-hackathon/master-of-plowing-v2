/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef SRC_APPLICATION_QRSCANNER_ABSTRACTQRCODERECEIVELISTENER_HPP_
#define SRC_APPLICATION_QRSCANNER_ABSTRACTQRCODERECEIVELISTENER_HPP_
#include "../../propertychange/PropertyChangeListener.hpp"
namespace nevonex
{
namespace app
{
namespace qrscanner
{

class AbstractQRCodeReceiveListener : public virtual ::nevonex::propertychange::PropertyChangeListener
{
public:
	AbstractQRCodeReceiveListener() = default;
	virtual ~AbstractQRCodeReceiveListener() = default;
protected:
	/**
	 * \brief For Handling Message Content Available.
	 *
	 * \param _content
	 * \return void
	 */
	virtual void onMessageRead(const std::string & _qrscan) = 0;
	

private:
	virtual void propertyChange(::nevonex::propertychange::PropertyChangeEvent<const ::ecore::EJavaObject&, const ::ecore::EJavaObject& > changeEvent) override;
};

} /* namespace cloud */
} /* namespace app */
} /* namespace qrscanner */

#endif
