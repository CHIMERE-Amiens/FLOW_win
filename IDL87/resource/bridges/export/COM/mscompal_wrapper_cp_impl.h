#ifndef _MSCOMPAL_WRAPPER_CP_IMPL_H_
#define _MSCOMPAL_WRAPPER_CP_IMPL_H_


template <class T>
class CProxy_IdlBmlWrapperEvents : public IConnectionPointImpl<T, &DIID_IBmlWRAPPEREVENTS, CComDynamicUnkArray>
{
public:
    HRESULT Fire_OnMouseDown(long mBtn, long kState, long xPos, long yPos, long* bForwardToIdl)
    {

        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[5];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);

                // COM reverses the order of parameters, so we have to put them in the
                // proper reverse order...
                pvars[4] = mBtn;
                pvars[3] = kState;
                pvars[2] = xPos;
                pvars[1] = yPos;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 5, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEDOWN, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

    HRESULT Fire_OnMouseUp(long mBtn, long kState, long xPos, long yPos, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[5];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[4] = mBtn;
                pvars[3] = kState;
                pvars[2] = xPos;
                pvars[1] = yPos;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 5, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEUP, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

    HRESULT Fire_OnMouseMove(long mBtn, long kState, long xPos, long yPos, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[5];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[4] = mBtn;
                pvars[3] = kState;
                pvars[2] = xPos;
                pvars[1] = yPos;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 5, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEMOVE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

    HRESULT Fire_OnMouseDoubleClick(long mBtn, long kState, long xPos, long yPos, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[5];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[4] = mBtn;
                pvars[3] = kState;
                pvars[2] = xPos;
                pvars[1] = yPos;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 5, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEDOUBLECLICK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

    HRESULT Fire_OnMouseWheel(long kState, long wDelta, long xPos, long yPos, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[5];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[4] = kState;
                pvars[3] = wDelta;
                pvars[2] = xPos;
                pvars[1] = yPos;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 5, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEWHEEL, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }


    HRESULT Fire_OnMouseEnter(void)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                DISPPARAMS disp = { NULL, NULL, 0, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEENTER, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        return varResult.scode;
    }

    HRESULT Fire_OnMouseExit(void)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                DISPPARAMS disp = { NULL, NULL, 0, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONMOUSEEXIT, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        return varResult.scode;
    }

    HRESULT Fire_OnKeyDown(long keyCode, long kState, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[3];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[2] = keyCode;
                pvars[1] = kState;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 3, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONKEYDOWN, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

    HRESULT Fire_OnKeyUp(long keyCode, long kState, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[3];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[2] = keyCode;
                pvars[1] = kState;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 3, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONKEYUP, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

    HRESULT Fire_OnSize(long width, long height, long* bForwardToIdl)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[3];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[2] = width;
                pvars[1] = height;
                pvars[0].SetByRef(bForwardToIdl);
                DISPPARAMS disp = { pvars, NULL, 3, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONSIZE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }


    HRESULT Fire_OnIDLNotify(BSTR bstr1, BSTR bstr2)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant* pvars = new CComVariant[2];
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                pvars[1] = bstr1;
                pvars[0] = bstr2;
                DISPPARAMS disp = { pvars, NULL, 2, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONIDLNOTIFY, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        delete[] pvars;
        return varResult.scode;
    }

	HRESULT Fire_OnIDLOutput(BSTR bstrOutput)
    {
        CComVariant varResult;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        CComVariant varIDLOutput;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
        {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
            if (pDispatch != NULL)
            {
                VariantClear(&varResult);
                varIDLOutput = bstrOutput;
                DISPPARAMS disp = { &varIDLOutput, NULL, 1, 0 };
                pDispatch->Invoke(IDLBML_EVENTID_ONIDLOUTPUT, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
            }
        }
        return varResult.scode;
    }

};
#endif