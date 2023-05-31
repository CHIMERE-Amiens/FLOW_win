// mscompal_wrapper_helper.h

//-----------------------------------------------------------------------------
BOOL IsVariantArray(VARIANT* pVar)
{
    if ( !pVar ) return FALSE;
    if ( (pVar->vt & VT_ARRAY) == VT_ARRAY )    return TRUE;
    if ( (pVar->vt & (VT_BYREF | VT_VARIANT)) == (VT_BYREF | VT_VARIANT) )
        return IsVariantArray(pVar->pvarVal);
    return FALSE;
}

//-----------------------------------------------------------------------------
BOOL IsArrayIdlSameMajority(VARIANT* pVar)
{
    if ( !pVar ) return FALSE;
    if ( (pVar->vt & VT_ARRAY) != VT_ARRAY )    return FALSE;
    if ( (pVar->vt & (VT_BYREF | VT_VARIANT)) == (VT_BYREF | VT_VARIANT) )
        pVar = pVar->pvarVal;

	BOOL bSameIdlMajority = pVar->parray->cDims & 0x8000;
	if ( bSameIdlMajority )
		pVar->parray->cDims &= ~0x8000;	// Clear flag
    return bSameIdlMajority;
}



//-----------------------------------------------------------------------------
BOOL Variant2Native(VARIANT* pVar, void* p )
{
    if ( !pVar || !p )
        return FALSE;
    if ( (pVar->vt & VT_ARRAY) == VT_ARRAY )
        return FALSE;
    if ( (pVar->vt & (VT_BYREF | VT_VARIANT)) == (VT_BYREF | VT_VARIANT) )
        pVar = pVar->pvarVal;

    VARTYPE vt = (pVar->vt & ~(VT_ARRAY|VT_BYREF));

    switch( vt )
    {
        case VT_UI1:
        case VT_I1:                 *((unsigned char*)p)    = V_UI1(pVar);      break;
        case VT_I2:                 *((short*)p)            = V_I2(pVar);       break;
        case VT_UINT:
        case VT_UI2:                *((unsigned short*)p)   = V_UI2(pVar);      break;
        case VT_INT:
        case VT_I4:                 *((long*)p)             = V_I4(pVar);       break;
        case VT_UI4:                *((unsigned long*)p)    = V_UI4(pVar);      break;
        case VT_I8:                 *((LONGLONG*)p)         = V_I8(pVar);       break;
        case VT_UI8:                *((ULONGLONG*)p)        = V_UI8(pVar);      break;
        case VT_R4:                 *((float*)p)            = V_R4(pVar);       break;
        case VT_R8:                 *((double*)p)           = V_R8(pVar);       break;

        case VT_BSTR:
            if ( pVar->bstrVal == NULL )
                return FALSE;
            *((BSTR*)p) = SysAllocString(pVar->bstrVal);
            break;

        case VT_USERDEFINED:
        case VT_ERROR:
        case VT_DATE:
        case VT_CY:
        case VT_BOOL:
        case VT_DISPATCH:
        case VT_UNKNOWN:
        default:
            return FALSE;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
HRESULT GetDispProperty(IDispatch* pDisp, char* pszProp, VARIANT* pVar)
{
    if ( !pDisp || !pszProp || !pVar )
        return E_POINTER;

    HRESULT hr;
    DISPID dispid = 0;
    hr = pDisp->GetIDsOfNames(
                        IID_NULL,
                        &CComBSTR(pszProp),
                        1, LOCALE_SYSTEM_DEFAULT,
                        &dispid);
    if ( FAILED(hr) )
        return hr;
    if ( !dispid )
        return DISP_E_UNKNOWNNAME;

    DISPPARAMS disp = { NULL, NULL, 0, 0 };
    hr = pDisp->Invoke(
                        dispid,
                        IID_NULL,
                        LOCALE_SYSTEM_DEFAULT,
                        DISPATCH_PROPERTYGET,
                        &disp,
                        pVar,
                        NULL, NULL);
    return hr;
}
