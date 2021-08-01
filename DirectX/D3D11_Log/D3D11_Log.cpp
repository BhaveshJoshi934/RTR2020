#include<stdio.h>
#include<d3d11.h>   // Like #include<GL/GL.h>
#include<Math.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")  // dxgi => DirectX Graphic Infrastructure => Abstracts all low level graphics driver API
                                 //analogous with WGL

int main(void)
{
    //local variable declarations
    IDXGIFactory *piDXGIFactory = NULL;   // Mi Itkya saglyanch pita ahe ki mala identitych nahiye...Pure abstract base class
                                          //pratyek interface iUnknown madhun inherit hoto
    IDXGIAdapter *piDXGIAdapter = NULL;

    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    HRESULT hr;
    char str[255];

    //code
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory),(void **)&piDXGIFactory);
    if(FAILED(hr))
    {
        printf("CreateDXGI() Failed!!!\n\n");
        goto cleanup;
    }

    if(piDXGIFactory->EnumAdapters(0,&piDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
    {
        printf("DXGIAdapter Can not be Found!!!\n\n");
        goto cleanup;
    }

    ZeroMemory((void*)&dxgiAdapterDesc,sizeof(DXGI_ADAPTER_DESC));
    hr = piDXGIAdapter->GetDesc(&dxgiAdapterDesc);

    WideCharToMultiByte(CP_ACP,0,dxgiAdapterDesc.Description,255,str,255,NULL,NULL); //code page:ANSI code page
    printf("Graphic Card Name = %s\n\n",str);
    //printf("Graphic Card VRAM = %I64d bytes\n\n",dxgiAdapterDesc.DedicatedVideoMemory);
    printf("VRAM in GB = %d GB\n\n",(int)(ceil(dxgiAdapterDesc.DedicatedVideoMemory/1024.0/1024.0/1024.0)));

    cleanup:
        if(piDXGIAdapter)
        {
            piDXGIAdapter->Release();
            piDXGIAdapter = NULL;
        }

        if(piDXGIFactory)
        {
            piDXGIFactory->Release();
            piDXGIFactory = NULL;
        }

    return(0);
}
