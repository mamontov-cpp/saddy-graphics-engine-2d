template< template<typename> class ARR, typename T > long findInArray(const class  ARR<T> &  v ,const T & me)
{
	long i;
	for (i=v.count()-1;i>-1;i--)
		if (v[i]==me)
			return i;
	return -1;
}
template<class T> inline long findInList(const class  hlist<T> &  v ,const T & me)
{
  return findInArray<hlist,T>(v,me);
}
template<class T> inline long findInDeque(const class  hdeque<T> &  v ,const T & me)
{
  return findInArray<hdeque,T>(v,me);
}
template<class T> void addAscendingUnique(class hdeque<T> & v, const T & me)
{
	long i;
	bool flag=true;
	bool conflag=true; //Flag of no presence of element in deque
	for (i=v.count()-1;(i>-1) && (flag) && (conflag);i--)
	{
		if (v[i]<me)
			flag=false;
		if (abs(v[i]-me)<2.1)
			conflag=false;
	}
	if (conflag)
		v.insert(me,(i<0)?((flag)?0:1):i+2);
}