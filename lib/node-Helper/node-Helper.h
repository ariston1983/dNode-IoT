template <typename T_ty> struct TypeInfo{
  static const char * name;
  typedef T_ty base;
};
template <typename T_ty> const char * TypeInfo<T_ty>::name = "unknown";

#define TYPE_NAME(var) TypeInfo< typeof(var) >::name
#define MAKE_TYPE_INFO(type)  template <> const char * TypeInfo<type>::name = #type;
