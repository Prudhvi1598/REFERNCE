#include<stdio.h>
#include<conio.h>
#include<tccore/custom.h>
#include<tccore/method.h>

#define DLLAPI _declspec(dllexport)

int status = 0;
//declaration of functions
extern DLLAPI int DLL_PROJECT_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
//Definition of function
extern DLLAPI int DLL_PROJECT_register_callbacks()
{
	
	status = CUSTOM_register_exit("DLL_PROJECT", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	status = CUSTOM_register_exit("DLL_PROJECT", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return status;
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----welcome dll registration process---- \n\n\n");
	printf("\n\n\n----login sucess---- \n\n\n");
	return status;
}
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n----logout sucess---- \n\n\n");
	return status;
}