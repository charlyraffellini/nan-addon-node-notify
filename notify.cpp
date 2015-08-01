#include <nan.h>

using namespace v8;

#include <string>
#include <libnotify/notify.h>

class Gtknotify : node::ObjectWrap {
  public:
    NotifyNotification *n;
    Gtknotify() {}
    ~Gtknotify() {}
    
    // Notification title
    std::string title;
    // Notification icon
    // 1) icon file path
    // 2) a 'stock' icon name. see /usr/share/icons
    // i.e. "terminal" or "chrome"
    std::string icon;
};

NAN_METHOD(Print) {
  NanScope();
  Local<String> str = args[0].As<String>();
  printf("%s\n", *String::Utf8Value(str));
  NanReturnUndefined();
}

NAN_METHOD(Length) {
  NanScope();
  Local<String> str = args[0].As<String>();
  int len = strlen(*String::Utf8Value(str));
  Local<Number> local_len = NanNew<Number>(len);
  NanReturnValue(local_len);
}

NAN_METHOD(Send) {
  NanScope();
  int timeout = NOTIFY_EXPIRES_DEFAULT;

  notify_init("node-notify");

  Gtknotify* gtknotify_instance = new Gtknotify();
  gtknotify_instance->title = "Node.js";
  gtknotify_instance->icon = "terminal";
  gtknotify_instance->n = notify_notification_new(gtknotify_instance->title.c_str(), "", gtknotify_instance->icon.c_str());

  Local<String> str = args[0].As<String>();

  notify_notification_update(gtknotify_instance->n, gtknotify_instance->title.c_str(), *String::Utf8Value(str), gtknotify_instance->icon.c_str()); // *v8str points to the C string
  notify_notification_set_timeout(gtknotify_instance->n, timeout);
  notify_notification_show(gtknotify_instance->n, NULL);
  NanReturnUndefined();
}


void Init(Handle<Object> exports) {
  exports->Set(NanNew("send"), NanNew<FunctionTemplate>(Send)->GetFunction());
  exports->Set(NanNew("print"), NanNew<FunctionTemplate>(Print)->GetFunction());
  exports->Set(NanNew("length"), NanNew<FunctionTemplate>(Length)->GetFunction());
}

NODE_MODULE(notify, Init)
