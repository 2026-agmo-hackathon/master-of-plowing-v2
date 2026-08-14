/*
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */
#ifndef NEVONEX_PROPERTYCHANGEEVENT_H_
#define NEVONEX_PROPERTYCHANGEEVENT_H_

#include <string>
namespace nevonex {
namespace propertychange {

/**
 * A "PropertyChange" event gets delivered whenever a bean changes a "bound"
 * or "constrained" property.  A PropertyChangeEvent object is sent as an
 * argument to the PropertyChangeListener.
 *
 * \author per9kor
 * \since 1.0
 *
 * \tparam S Source class type.
 * \tparam T Value class type.
 */
template <class S, class T>
class PropertyChangeEvent
{
private:
	S source;
	std::string propertyName;
	T oldValue;
	T newValue;

public:
	explicit PropertyChangeEvent(const S & _src, const std::string & _property, const T & _old, const T & _new) :
		source(_src), propertyName(_property), oldValue(_old), newValue(_new)
	{

	};
	inline ~PropertyChangeEvent() = default;
	inline S getSource() const
	{
		return this->source;
	};
	inline std::string getPropertyName() const
	{
		return this->propertyName;
	};
	inline T getOldValue() const
	{
		return this->oldValue;
	};
	inline T getNewValue() const
	{
		return this->newValue;
	};
};

} // propertychange
} // nevonex

#endif /* NEVONEX_PROPERTYCHANGEEVENT_H_ */
