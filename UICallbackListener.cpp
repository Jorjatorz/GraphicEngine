#include "UICallbackListener.h"

//#include "MemberFunctionSlot.h"

#include <iostream>

UICallbackListener::UICallbackListener()
{
	mAttachedView = NULL;
}


UICallbackListener::~UICallbackListener()
{
	tReciversMap::iterator it;
	for (it = mReciversMap.begin(); it != mReciversMap.end(); ++it)
	{
		delete it->second;
	}
	mReciversMap.clear();
}

void UICallbackListener::bind(Awesomium::WebView* view)
{
	mAttachedView = view;

	//Create a JS global object named "ingame"
	mJSValue = mAttachedView->CreateGlobalJavascriptObject(Awesomium::WSLit("ingame"));
	//Set this as the js handler
	mAttachedView->set_js_method_handler(this);
}

void UICallbackListener::OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args)
{
	Awesomium::JSObject& mGlobalJSObject = mJSValue.ToObject();

	tReciversMap::iterator it;
	for (it = mReciversMap.begin(); it != mReciversMap.end(); ++it)
	{
		if (mGlobalJSObject.remote_id() == remote_object_id)
		{
			if (method_name == Awesomium::WSLit(it->first.c_str()))
			{
				//The JSfunction was called, call the attached function
				it->second->executeAttachedFunction();
				return;
			}
		}
	}
}

Awesomium::JSValue UICallbackListener::OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args)
{
		return Awesomium::JSValue::Undefined();
}

Awesomium::JSValue UICallbackListener::getJSValue(std::string elementNane, std::string jsPropertyName)
{
	//Care with performance!!!
	//In documentation it checks if value is a object but we skipt that
	std::string command = "document.getElementById('" + elementNane + "')";
	Awesomium::JSValue result = mAttachedView->ExecuteJavascriptWithResult(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	if (result.IsObject())
	{
		return result.ToObject().GetProperty(Awesomium::WSLit(jsPropertyName.c_str()));
	}

	return Awesomium::JSValue::Undefined();
}

void UICallbackListener::setJSValue(std::string elementNane, std::string jsPropertyName, std::string newValue)
{
	//Build up the command
	std::string command = "document.getElementById('" + elementNane + "')." + jsPropertyName + " = '" + newValue + "';";
	mAttachedView->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	//VERY SLOW PERFORMANCE
	/*
	std::string command = "document.getElementById('" + elementNane + "')";
	Awesomium::JSValue result = mAttachedView->ExecuteJavascriptWithResult(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	if (result.IsObject())
	{
		result.ToObject().SetProperty(Awesomium::WSLit(jsPropertyName.c_str()), Awesomium::WSLit(newValue.c_str()));
	}*/
}

void UICallbackListener::setJSValue(std::string elementNane, std::string jsPropertyName, real newValue)
{
	std::string command = "document.getElementById('" + elementNane + "')." + jsPropertyName + " = '" + std::to_string(newValue) + "';";
	mAttachedView->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	//VERY SLOW PERFORMANCE
	/*
	std::string command = "document.getElementById('" + elementNane + "')";
	Awesomium::JSValue result = mAttachedView->ExecuteJavascriptWithResult(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
	if (result.IsObject())
	{
		result.ToObject().SetProperty(Awesomium::WSLit(jsPropertyName.c_str()), Awesomium::JSValue(newValue));
	}*/
}
void UICallbackListener::setJSValue(std::string elementNane, std::string jsPropertyName, bool newValue)
{
	//Same as other but without "
	std::string command = "document.getElementById('" + elementNane + "')." + jsPropertyName + " = " + std::to_string(newValue) + ";";
	mAttachedView->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WSLit(""));
}
