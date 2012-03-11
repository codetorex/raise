#ifndef TDATAMAP_H
#define TDATAMAP_H

/*class UserDataStorage
{
public:

	template<typename T> struct Value { Value( const T& v ){ val = v; }; T val; };

	template<typename T>
	void Add( T var )
	{
		if ( Exists<T>() ) return;

		Value<T>* val = new Value<T>( var );
		m_List[ typeid( var ).name() ] = (void*) val;
	};

	template<typename T>
	T& Get()
	{
		Value<T>* v = (Value<T>*)m_List[ typeid( T ).name() ];
		return v->val;
	};

	template<typename T>
	bool Exists()
	{
		return m_List.find( typeid( T ).name() ) != m_List.end();
	};


	std::map< String, void*>    m_List;

	MyInformationClass* pInfo = new Info();
	pMyThing->UserData.Add( pInfo );

	MyInformationClass* pValue = pMyThing->UserData.Get<MyInformationClass*>();

};*/

#endif