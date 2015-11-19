/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : Folder Watch
 #	author : miyako
 #	2015/01/26
 #
 # --------------------------------------------------------------------------------*/

#include "4DPluginAPI.h"
#include "4DPlugin.h"

process_number_t MONITOR_FOLDER_METHOD_PROCESS_ID;
process_stack_size_t MONITOR_FOLDER_STACK_SIZE;
process_name_t MONITOR_FOLDER_METHOD_PROCESS_NAME;

method_id_t MONITOR_FOLDER_METHOD_ID;

bool MONITOR_FOLDER_METHOD_PROCESS_SHOULD_TERMINATE;
bool MONITOR_FOLDER_METHOD_PROCESS_SHOULD_EXECUTE_METHOD;

C_TEXT MONITOR_FOLDER_WATCH_PATH;
C_TEXT MONITOR_FOLDER_WATCH_METHOD;
C_TEXT MONITOR_FOLDER_METHOD_PROCESS_NAME_INTERNAL;

void generateUuid(C_TEXT &returnValue){
    
#if VERSIONMAC
#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1080
    returnValue.setUTF16String([[[NSUUID UUID]UUIDString]stringByReplacingOccurrencesOfString:@"-" withString:@""]);
#else
    CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
    NSString *uuid_str = (NSString *)CFUUIDCreateString(kCFAllocatorDefault, uuid);
    returnValue.setUTF16String([uuid_str stringByReplacingOccurrencesOfString:@"-" withString:@""]);
#endif
#else
    
#endif
}

#pragma mark -

bool IsProcessOnExit(){    
    C_TEXT name;
    PA_long32 state, time;
    PA_GetProcessInfo(PA_GetCurrentProcessNumber(), name, &state, &time);
    CUTF16String procName(name.getUTF16StringPtr());
    CUTF16String exitProcName((PA_Unichar *)"$\0x\0x\0\0\0");
    return (!procName.compare(exitProcName));
}

void OnStartup(){
    CUTF8String name((const uint8_t *)"$FOLDER_WATCH");
    MONITOR_FOLDER_METHOD_PROCESS_NAME_INTERNAL.setUTF8String(&name);
    MONITOR_FOLDER_METHOD_PROCESS_NAME = (process_name_t)MONITOR_FOLDER_METHOD_PROCESS_NAME_INTERNAL.getUTF16StringPtr();
    
    MONITOR_FOLDER_METHOD_PROCESS_ID = 0;
    MONITOR_FOLDER_STACK_SIZE = 0;
    MONITOR_FOLDER_METHOD_ID = 0;
}

void OnCloseProcess(){
    if(IsProcessOnExit()){
        listenerLoopFinish();
    }
}

#pragma mark -

void listenerLoop(){
    
    MONITOR_FOLDER_METHOD_PROCESS_SHOULD_EXECUTE_METHOD = false;
    MONITOR_FOLDER_METHOD_PROCESS_SHOULD_TERMINATE = false;
        
    while(!MONITOR_FOLDER_METHOD_PROCESS_SHOULD_TERMINATE)
    { 
        PA_YieldAbsolute();
        
        if(MONITOR_FOLDER_METHOD_PROCESS_SHOULD_EXECUTE_METHOD){
            
            MONITOR_FOLDER_METHOD_PROCESS_SHOULD_EXECUTE_METHOD = false;
            
            C_TEXT processName;
            generateUuid(processName);
            PA_NewProcess((void *)listenerLoopExecuteMethod, 
                          MONITOR_FOLDER_STACK_SIZE, 
                          (PA_Unichar *)processName.getUTF16StringPtr());        

        }
        
        if(!MONITOR_FOLDER_METHOD_PROCESS_SHOULD_TERMINATE){
            PA_FreezeProcess(PA_GetCurrentProcessNumber());
        }else{
            MONITOR_FOLDER_METHOD_PROCESS_ID = 0;
        }
    }
    PA_KillProcess();
}

#if VERSIONMAC 
std::string MONITOR_FOLDER_WATCH_PATH_POSIX;
const char *MONITOR_FOLDER_DISPATCH_QUEUE_NAME = "FOLDER_WATCH_QUEUE";
dispatch_queue_t MONITOR_FOLDER_DISPATCH_QUEUE;
dispatch_source_t MONITOR_FOLDER_DISPATCH_SOURCE;
int MONITOR_FOLDER_DESCRIPTOR = 0;
dispatch_source_vnode_flags_t MONITOR_FOLDER_DISPATCH_FLAGS = DISPATCH_VNODE_WRITE|DISPATCH_VNODE_DELETE|DISPATCH_VNODE_EXTEND;
#else

#endif 

void listenerLoopStart(){

    if(!MONITOR_FOLDER_METHOD_PROCESS_ID){
        
        MONITOR_FOLDER_METHOD_PROCESS_ID = PA_NewProcess((void *)listenerLoop, 
                                                         MONITOR_FOLDER_STACK_SIZE, 
                                                         MONITOR_FOLDER_METHOD_PROCESS_NAME);  
        
#if VERSIONMAC  
        
        MONITOR_FOLDER_DESCRIPTOR = open(MONITOR_FOLDER_WATCH_PATH_POSIX.c_str(), O_EVTONLY);
        MONITOR_FOLDER_DISPATCH_QUEUE = dispatch_queue_create(MONITOR_FOLDER_DISPATCH_QUEUE_NAME, 0);
        MONITOR_FOLDER_DISPATCH_SOURCE = dispatch_source_create(DISPATCH_SOURCE_TYPE_VNODE,
                                                                MONITOR_FOLDER_DESCRIPTOR,
                                                                MONITOR_FOLDER_DISPATCH_FLAGS,
                                                                MONITOR_FOLDER_DISPATCH_QUEUE);
        
        dispatch_source_set_event_handler(MONITOR_FOLDER_DISPATCH_SOURCE, ^(){
            listenerLoopExecute();
        });
        
        dispatch_source_set_cancel_handler(MONITOR_FOLDER_DISPATCH_SOURCE, ^() {
            close(MONITOR_FOLDER_DESCRIPTOR);
            MONITOR_FOLDER_DESCRIPTOR = 0; 
        });
        
        dispatch_resume(MONITOR_FOLDER_DISPATCH_SOURCE);        
#else
        
#endif         
    }
}

void listenerLoopFinish(){

    if(MONITOR_FOLDER_METHOD_PROCESS_ID){
        //uninstall handler 
#if VERSIONMAC         
        dispatch_source_set_event_handler_f(MONITOR_FOLDER_DISPATCH_SOURCE, NULL);      
        dispatch_source_cancel(MONITOR_FOLDER_DISPATCH_SOURCE);
#else
        
#endif 
        //set flags
        MONITOR_FOLDER_METHOD_PROCESS_SHOULD_TERMINATE = true;
        MONITOR_FOLDER_METHOD_PROCESS_SHOULD_EXECUTE_METHOD = false;
        PA_YieldAbsolute();
        //tell listener to die
        while(MONITOR_FOLDER_METHOD_PROCESS_ID){
            PA_YieldAbsolute();
            PA_UnfreezeProcess(MONITOR_FOLDER_METHOD_PROCESS_ID);
        }
    }
} 

void listenerLoopExecute(){
    MONITOR_FOLDER_METHOD_PROCESS_SHOULD_TERMINATE = false;
    MONITOR_FOLDER_METHOD_PROCESS_SHOULD_EXECUTE_METHOD = true;
    PA_UnfreezeProcess(MONITOR_FOLDER_METHOD_PROCESS_ID);
}

void listenerLoopExecuteMethod(){
    
    if(MONITOR_FOLDER_METHOD_ID){
        
        PA_Variable	params[1];
        params[0] = PA_CreateVariable(eVK_Unistring);	
        
        PA_Unistring path = PA_CreateUnistring((PA_Unichar *)MONITOR_FOLDER_WATCH_PATH.getUTF16StringPtr());
        PA_SetStringVariable(&params[0], &path);
        
        PA_ExecuteMethodByID(MONITOR_FOLDER_METHOD_ID, params, 1);
        
        PA_DisposeUnistring(&path);
        
        PA_ClearVariable(&params[0]);//added 15.11.19
    }
    
}

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher(PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
        case kInitPlugin :
        case kServerInitPlugin :            
            OnStartup();
            break;
            
        case kCloseProcess :            
            OnCloseProcess();
            break;
            
// --- Settings

		case 1 :
			FOLDER_Set_watch_path(pResult, pParams);
			break;

		case 2 :
			FOLDER_Get_watch_path(pResult, pParams);
			break;

		case 3 :
			FOLDER_Set_watch_method(pResult, pParams);
			break;

		case 4 :
			FOLDER_Get_watch_method(pResult, pParams);
			break;

	}
}

#pragma mark -

// ----------------------------------- Settings -----------------------------------

int MONITOR_FOLDER_NOT_FOLDER_ERROR = -1;
int MONITOR_FOLDER_INVALID_PATH_ERROR = -2;
int MONITOR_FOLDER_INVALID_METHOD_NAME_ERROR = -3;

void FOLDER_Set_watch_path(sLONG_PTR *pResult, PackagePtr pParams)
{ 
    C_TEXT Param1;
	C_LONGINT returnValue;

	Param1.fromParamAtIndex(pParams, 1);
    
    if(!Param1.getUTF16Length()){
        
        returnValue.setIntValue(1);
        
        if(MONITOR_FOLDER_WATCH_PATH.getUTF16Length()){
            
            MONITOR_FOLDER_WATCH_PATH.setUTF16String(Param1.getUTF16StringPtr(), Param1.getUTF16Length());
            
            if(MONITOR_FOLDER_WATCH_METHOD.getUTF16Length()){
                listenerLoopFinish();    
            } 
        }

    }else{
    
#if VERSIONMAC    
        
        BOOL isDirectory = false;    
        NSString *path = Param1.copyPath();
        NSString *pathHFS = Param1.copyUTF16String();
        
        if([[NSFileManager defaultManager]fileExistsAtPath:path isDirectory:&isDirectory]){
            
            if(isDirectory){
                
                returnValue.setIntValue(1); 
                MONITOR_FOLDER_WATCH_PATH.setUTF16String(pathHFS); 
                MONITOR_FOLDER_WATCH_PATH_POSIX = (const char *)[path UTF8String];
                
                if(MONITOR_FOLDER_WATCH_METHOD.getUTF16Length()){
                    listenerLoopFinish();
                    listenerLoopStart();            
                }
                
            }else{
                returnValue.setIntValue(MONITOR_FOLDER_NOT_FOLDER_ERROR);
            }
            
        }else{
            returnValue.setIntValue(MONITOR_FOLDER_INVALID_PATH_ERROR);
        }
        
        [path release];
        [pathHFS release];        
#else
        
#endif    
        
    }
    
	returnValue.setReturn(pResult);
}

void FOLDER_Set_watch_method(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT returnValue;

	Param1.fromParamAtIndex(pParams, 1);

    if(!Param1.getUTF16Length()){
        
        returnValue.setIntValue(1);
        
        if(MONITOR_FOLDER_WATCH_METHOD.getUTF16Length()){
            
            MONITOR_FOLDER_WATCH_METHOD.setUTF16String(Param1.getUTF16StringPtr(), Param1.getUTF16Length());
            MONITOR_FOLDER_METHOD_ID = 0;
            
            if(MONITOR_FOLDER_WATCH_PATH.getUTF16Length()){
                listenerLoopFinish();    
            }            
        }

    }else{  
        
        method_id_t methodId = PA_GetMethodID((PA_Unichar *)Param1.getUTF16StringPtr());
        
        if(methodId){
            
            returnValue.setIntValue(1);
            
            if(methodId != MONITOR_FOLDER_METHOD_ID){
                
                MONITOR_FOLDER_WATCH_METHOD.setUTF16String(Param1.getUTF16StringPtr(), Param1.getUTF16Length());
                MONITOR_FOLDER_METHOD_ID = methodId;
                
                if(MONITOR_FOLDER_WATCH_PATH.getUTF16Length()){
                    listenerLoopFinish();
                    listenerLoopStart();                  
                }

            }
            
        }else{
            
            returnValue.setIntValue(MONITOR_FOLDER_INVALID_METHOD_NAME_ERROR);
            
        }
      
    }
  
	returnValue.setReturn(pResult);
}

void FOLDER_Get_watch_path(sLONG_PTR *pResult, PackagePtr pParams)
{
    MONITOR_FOLDER_WATCH_PATH.setReturn(pResult);
}

void FOLDER_Get_watch_method(sLONG_PTR *pResult, PackagePtr pParams)
{
	MONITOR_FOLDER_WATCH_METHOD.setReturn(pResult);
}
