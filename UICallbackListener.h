#ifndef UICALLBACKLISTENER_H
#define UICALLBACKLISTENER_H

#include "Definitions.h"
#include "MemberFunctionSlot.h"
#include "ListenerReciver.h"

#include <map>
#include <string>

class UICallbackListener : public Awesomium::JSMethodHandler
{
public:
	UICallbackListener();
	~UICallbackListener();


	void bind(Awesomium::WebView* view);

	//Callbacks
	//Inner function of the global JS script, this binds that jsFunction to the tReciversMap - If the function is executed the attached function in the map is executed - tempaltes in .h
	template <typename T>
	void addFunctionCallback(std::string jsFunctionName, void (T::*functionToExecutePtr)(), T* obj)
	{
		Awesomium::JSObject& mGlobalJSObject = mJSValue.ToObject();
		tReciversMap::iterator it = mReciversMap.find(jsFunctionName);
		if (it == mReciversMap.end())
		{
			mGlobalJSObject.SetCustomMethod(Awesomium::WSLit(jsFunctionName.c_str()), false);

			ListenerReciver* reciver = new ListenerReciver(functionToExecutePtr, obj);
			mReciversMap.insert(std::pair<std::string, ListenerReciver*>(jsFunctionName, reciver));
		}
	}

	//Return the JSValue of that object
	Awesomium::JSValue getJSValue(std::string elementNane, std::string jsPropertyName);
	void setJSValue(std::string elementNane, std::string jsPropertyName, std::string newValue);
	void setJSValue(std::string elementNane, std::string jsPropertyName, real newValue);
	void setJSValue(std::string elementNane, std::string jsPropertyName, bool newValue);


	virtual void OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args);
	virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args);

private:
	Awesomium::WebView* mAttachedView;
	Awesomium::JSValue mJSValue;

	typedef std::map<std::string, ListenerReciver*> tReciversMap;
	tReciversMap mReciversMap;
};

#endif