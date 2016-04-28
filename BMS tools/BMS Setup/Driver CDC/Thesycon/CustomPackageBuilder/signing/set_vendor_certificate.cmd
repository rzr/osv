@rem
@rem Set the variables below according to your code signing certificate.
@rem The examples shown are for a certificate issued by VeriSign.
@rem As an alternative, set these variables in the environment of your build machine.
@rem So you don't need to edit this script.
@rem
@rem The certificate specified in VENDOR_CERTIFICATE must be installed in 
@rem the local Personal certificate store. Run certmgr.msc to check the Personal store.
@rem
@rem VENDOR_CERTIFICATE is the "issued to" name of the certificate as shown in certmgr.msc
@rem This needs to be set to the correct name.
@rem
@rem CROSS_CERTIFICATE is the file name of the cross certificate for your 
@rem certificate provider. The default shown below is correct for a VeriSign certificate.
@rem
@rem SIGNTOOL_TIMESTAMP_URL is the URL of a trusted timestamp server.
@rem The default shown below is correct for a VeriSign certificate.
@rem 


@rem 
@rem Activate the following line and specify the name of your vendor certificate.
@rem Note: If your company name contains special characters <, >, |, &, ^ then each of these 
@rem characters must be preceded by the escape character (^), e.g.: MyCompany ^& Co.
@rem 
rem @set VENDOR_CERTIFICATE=MyCompany Ltd.

@rem 
@rem You need to specify the cross-certificate valid for your vendor certificate.
@rem The cross-cert specified below is valid for a vendor cert issued by VeriSign only.
@rem If you are using a vendor cert issued by another CA, you need a different cross cert.
@rem For latest information on cross certificates, see the article 
@rem "Cross-Certificates for Kernel Mode Code Signing" in the MSDN library
@rem http://msdn.microsoft.com/en-us/library/windows/hardware/gg487315.aspx
@rem or ask you certificate provider which cross cert is to be used.
@rem The cross certificate file must be placed into the same directory where 
@rem this script resides.
@rem 
@set CROSS_CERTIFICATE=MSCV-VSClass3.cer

@rem 
@rem Timestamp server to be used.
@rem A different server may be required if your vendor cert was not issued by VeriSign.
@rem 
@set SIGNTOOL_TIMESTAMP_URL=http://timestamp.verisign.com/scripts/timestamp.dll

