/**
 * \file Event.h
 * \brief The event library in paper library, this head will provide a event bus liked
 * event accomplish for event communication
 */

#pragma once

#include <functional>
#include <list>
#include <memory>
#include <mutex>

/**
 * The wrapper of std::function
 */
template <class... Parameters> using PDelegate = std::function<void(Parameters...)>;

/**
 * The connection unit of PSignal
 * @tparam Parameters The function parameters
 */
template <class... Parameters> class PConnectUnit {
public:
	/**
	 * Construct the connect unit with the function delegate
	 * @param Function The delegate instance to the function
	 */
	explicit PConnectUnit(PDelegate<Parameters...> Function) : _blocked(false) {
		_function = Function;
	}

public:
	/**
	 * Get the function delegate instance of the object
	 * @return The delegate instance
	 */
	inline PDelegate<Parameters...> *GetFunction() {
		return &_function;
	}
	/**
	 * Check the unit block status
	 * @return If return true, the unit is now on blocked status,
	 * otherwise not
	 */
	bool Blocked() {
		return _blocked;
	}
	/**
	 * Set the unit block status
	 * @param Status If the status is true, this unit will be blocked,
	 * otherwise not
	 */
	void SetBlock(bool Status) {
		_blocked = Status;
	}

private:
	PDelegate<Parameters...> _function;
	bool					 _blocked;
};

/**
 * The function connection unit
 * @tparam Parameters The function parameters
 */
template <class... Parameters> class PFunctionConnection : public PConnectUnit<Parameters...> {
public:
	/**
	 * Alias of raw function pointer
	 */
	using FunctionPointer = void (*)(Parameters...);

public:
	/**
	 * Construct the function connection unit by a raw C function pointer
	 * @param InitFunction The function pointer
	 */
	explicit PFunctionConnection(FunctionPointer InitFunction)
		: PConnectUnit<Parameters...>(PDelegate<Parameters...>(InitFunction)) {
		_pointer = InitFunction;
	}

	/**
	 * Get the raw C pointer of this unit
	 * @return The function pointer formatted in raw C style
	 */
	inline FunctionPointer GetPointer() {
		return _pointer;
	}

private:
	FunctionPointer _pointer;
};
/**
 * The class based function connection unit
 * @tparam Parameters The function parameters
 */
template <class ObjectClass, class... Parameter> class PClassConnection : public PConnectUnit<Parameter...> {
public:
	/**
	 * The alisa for member function pointer
	 */
	using FunctionPointer = void (ObjectClass::*)(Parameter...);

public:
	/**
	 * Construct a class connection unit by the object instance and
	 * a member function pointer
	 * @param Object The object pointer referred to the instance
	 * @param Function The member function pointer
	 */
	PClassConnection(ObjectClass *Object, FunctionPointer Function)
		: PConnectUnit<Parameter...>([Object, Function](Parameter... Args) { (*Object.*Function)(Args...); }) {
		_objectPointer = Object;
		_function	   = Function;
	}

	/**
	 * Get the object instance pointer
	 * @return The object instance pointer
	 */
	inline void *GetObject() {
		return _objectPointer;
	}
	/**
	 * Get the raw C pointer of this unit
	 * @return The function pointer formatted in raw C style
	 */
	inline FunctionPointer GetPointer() {
		return _function;
	}

private:
	ObjectClass	   *_objectPointer;
	FunctionPointer _function;
};

/**
 * Event operation type enum class
 */
enum class PEventOperation {
	Lookup,
	Delete,
	Block,
	Unblock
};

/**
 * The event class (Observer mode)
 * @tparam Parameters The event calling parameters type
 */
template <class... Parameters> class PEvent {
public:
	using FunctionPointer									= void (*)(Parameters...);
	template <class ObjectClass> using ClassFunctionPointer = void (ObjectClass::*)(Parameters...);

public:
	/**
	 * By default, the PEvent will maintain a list of PConnectionUnit in
	 * std::shared_ptr
	 */
	PEvent() {
		_slots = new std::list<std::shared_ptr<PConnectUnit<Parameters...>>>;
	}
	~PEvent() {
		delete _slots;
	}

public:
	/**
	 * Register handle function
	 * @param Function The function pointer
	 */
	inline void Connect(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		if (!Operation(Function, PEventOperation::Lookup)) {
			std::shared_ptr<PConnectUnit<Parameters...>> pointer(new PFunctionConnection(Function));
			_slots->emplace_back(pointer);
		}
	}
	/**
	 * Register class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template <class ObjectType> inline void Connect(ObjectType *Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		if (!Operation(Object, Function, PEventOperation::Lookup)) {
			std::shared_ptr<PConnectUnit<Parameters...>> pointer(
				new PClassConnection<ObjectType, Parameters...>(Object, Function));
			_slots->emplace_back(pointer);
		}
	}
	/**
	 * Unregister handle function
	 * @param Function The function pointer
	 */
	inline void Disconnect(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Function, PEventOperation::Delete);
	}
	/**
	 * Unregister class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template <class ObjectType> inline void Disconnect(ObjectType *Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Object, Function, PEventOperation::Delete);
	}
	/**
	 * Block handle function
	 * @param Function The function pointer
	 */
	inline void Block(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Function, PEventOperation::Block);
	}
	/**
	 * Block class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template <class ObjectType> inline void Block(ObjectType *Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Object, Function, PEventOperation::Block);
	}
	/**
	 * Unblock handle function
	 * @param Function The function pointer
	 */
	inline void Unblock(FunctionPointer Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Function, PEventOperation::Unblock);
	}
	/**
	 * Unblock class handle function
	 * @tparam ObjectType The class type
	 * @param Object The object instance for function emit
	 * @param Function The class function
	 */
	template <class ObjectType> inline void Unblock(ObjectType *Object, ClassFunctionPointer<ObjectType> Function) {
		std::lock_guard<std::mutex> guard(_lock);
		Operation(Object, Function, PEventOperation::Unblock);
	}

public:
	/**
	 * Publish the event to event listener
	 * @param Args The args to publish the event
	 */
	void Emit(Parameters... Args) {
		for (auto function = _slots->begin(); function != _slots->end(); ++function) {
			if (function->get()->Blocked()) {
				continue;
			}

			auto pointer = function->get()->GetFunction();

			(*pointer)(Args...);
		}
	}
	/**
	 * Publish the event to event listener
	 * @param Args The args to publish the event
	 */
	void operator()(Parameters... Args) {
		Emit(Args...);
	}

private:
	/**
	 * Conduct Operation of slots for function connection
	 * @return If the target exists in slots, return true.
	 */
	 bool Operation(FunctionPointer Function, const PEventOperation &Operation) {
		for (auto iterator = _slots->begin(); iterator != _slots->end();) {
			auto connectFunction = static_cast<PFunctionConnection<Parameters...> *>((*iterator).get());
			if (connectFunction->GetPointer() == Function) {
				switch (Operation) {
				case PEventOperation::Delete: {
					_slots->erase(iterator++);

					break;
				}
				case PEventOperation::Block: {
					connectFunction->SetBlock(true);
					++iterator;

					break;
				}
				case PEventOperation ::Unblock: {
					connectFunction->SetBlock(false);
					++iterator;

					break;
				}
				default: {
					break;
				}
				}

				return true;
			} else {
				++iterator;
			}
		}
		return false;
	}
	/**
	 * Conduct Operation of slots for class connection
	 * @return If the target exists in slots, return true.
	 */
	template <class ObjectType>
	bool Operation(ObjectType *Object, ClassFunctionPointer<ObjectType> Function, const PEventOperation &Operation) {
		for (auto iterator = _slots->begin(); iterator != _slots->end();) {
			auto connectFunction = static_cast<PClassConnection<ObjectType, Parameters...> *>((*iterator).get());
			if (connectFunction->GetPointer() == Function && connectFunction->GetObject() == Object) {
				switch (Operation) {
				case PEventOperation::Delete: {
					_slots->erase(iterator++);

					break;
				}
				case PEventOperation::Block: {
					connectFunction->SetBlock(true);
					++iterator;

					break;
				}
				case PEventOperation ::Unblock: {
					connectFunction->SetBlock(false);
					++iterator;

					break;
				}
				default: {
					break;
				}
				}

				return true;
			} else {
				++iterator;
			}
		}
		return false;
	}

private:
	std::mutex												 _lock;
	std::list<std::shared_ptr<PConnectUnit<Parameters...>>> *_slots;
};