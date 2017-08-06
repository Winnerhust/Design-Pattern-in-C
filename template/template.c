#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_STEP_NUM 16
#define MAX_RUN_WIZARD_NUM 32

typedef int (*WizardsStep)(void *pstOption);

typedef struct {
    char szWizardName[32];
    WizardsStep steps[MAX_STEP_NUM];
} AppWizards;

char g_szWizardNameList[MAX_RUN_WIZARD_NUM][32];

typedef struct {
    char *szAppName;
    int keeppkg;
} AppWizardsOptions;

int AppDownload(void *pstOption)
{
    AppWizardsOptions *p = (AppWizardsOptions *)pstOption;
    assert(p);

    printf("download %s begin...\n", p->szAppName);
    printf("download %s successfully...\n", p->szAppName);

    return 0;
}

int AppInstall(void *pstOption)
{
    AppWizardsOptions *p = (AppWizardsOptions *)pstOption;
    assert(p);

    printf("install %s begin...\n", p->szAppName);
    printf("install %s successfully...\n", p->szAppName);
    if (p->keeppkg)
    {
        printf("keep the install file of %s\n", p->szAppName);
    }
    return 0;
}

int AppStart(void *pstOption)
{
    AppWizardsOptions *p = (AppWizardsOptions *)pstOption;
    assert(p);

    printf("start %s begin...\n", p->szAppName);
    printf("start %s successfully...\n", p->szAppName);

    return 0;
}

void AppFastInstallWizardsInit(AppWizards *p, char *pszAppName)
{
    memset(p, 0, sizeof(AppWizards));
    strncpy(p->szWizardName, pszAppName, sizeof(p->szWizardName) - 1);

    p->steps[0] = AppDownload;
    p->steps[1] = AppInstall;
    p->steps[2] = AppStart;

    return;
}

void AppEasyInstallWizardsInit(AppWizards *p, char *pszAppName)
{
    memset(p, 0, sizeof(AppWizards));
    strncpy(p->szWizardName, pszAppName, sizeof(p->szWizardName) - 1);
    p->steps[0] = AppDownload;
    p->steps[1] = AppInstall;
    return;
}

void AppDownLoadWizardsInit(AppWizards *p, char *pszAppName)
{
    memset(p, 0, sizeof(AppWizards));
    strncpy(p->szWizardName, pszAppName, sizeof(p->szWizardName) - 1);
    p->steps[0] = AppDownload;

    return;
}

void AppInstallWizardsInit(AppWizards *p, char *pszAppName)
{
    memset(p, 0, sizeof(AppWizards));
    strncpy(p->szWizardName, pszAppName, sizeof(p->szWizardName) - 1);
    p->steps[0] = AppInstall;

    return;
}

void AppStartlWizardsInit(AppWizards *p, char *pszAppName)
{
    memset(p, 0, sizeof(AppWizards));
    strncpy(p->szWizardName, pszAppName, sizeof(p->szWizardName) - 1);
    p->steps[0] = AppStart;

    return;
}

int addRunWizard(char *pszWizardName)
{
    int i = 0;
    for (i = 0; i < MAX_RUN_WIZARD_NUM; i++)
    {
        if (strcmp(g_szWizardNameList[i], pszWizardName) == 0)
        {
            return -1;
        }
    }

    for (i = 0; i < MAX_RUN_WIZARD_NUM; i++)
    {
        if (g_szWizardNameList[i][0] == '\0')
        {
            snprintf(g_szWizardNameList[i],
                     sizeof(g_szWizardNameList[i]),
                     "%s",
                     pszWizardName);
            return 0;
        }
    }

    return -2;
}

void removeRunWizard(char *pszWizardName)
{
    int i;
    for (i = 0; i < MAX_RUN_WIZARD_NUM; i++)
    {
        if (strcmp(g_szWizardNameList[i], pszWizardName) == 0)
        {
            g_szWizardNameList[i][0] = '\0';
            return;
        }
    }

    return;
}

int AppRunWizard(AppWizards *app, void *pstOption)
{
    int i = 0;
    int nRet;

    if (addRunWizard(app->szWizardName))
    {
        printf("failed: there is another same wizard named (%s)\n",
               app->szWizardName);
        return -1;
    }

    WizardsStep step;
    step = app->steps[i++];
    while ((step != NULL) && (i <= MAX_STEP_NUM))
    {
        nRet = step(pstOption);
        if (nRet != 0)
        {
            return nRet;
        }
        step = app->steps[i++];
    }

    removeRunWizard(app->szWizardName);

    return 0;
}

void AppWizardsOptionInit(AppWizardsOptions *option,
                          char *pszAppName,
                          int keeppkg)
{
    option->szAppName = pszAppName;
    option->keeppkg = keeppkg;

    return;
}

int main(int argc, char *argv[])
{
    AppWizards w;
    AppWizardsOptions o;
    char *pszAppName;
    if (argc < 3) {
        printf("usage: %s {e|i|f|d} appname\n", argv[0]);
        return 1;
    }
    pszAppName = argv[2];

    if (argv[1][0] == 'e') {
        AppEasyInstallWizardsInit(&w, pszAppName);
        AppWizardsOptionInit(&o, pszAppName, 1);
    } else if (argv[1][0] == 'i') {
        AppInstallWizardsInit(&w, pszAppName);
        AppWizardsOptionInit(&o, pszAppName, 1);
    } else if (argv[1][0] == 'f') {
        AppFastInstallWizardsInit(&w, pszAppName);
        AppWizardsOptionInit(&o, pszAppName, 1);
    } else {
        AppDownLoadWizardsInit(&w, pszAppName);
        AppWizardsOptionInit(&o, pszAppName, 1);
    }

    // 模拟多个向导程序运行时，同名的向导只能运行一个
    // 因此，输入app1时会提示有同名的向导程序
    // 输入其他名称的向导程序则能正常运行
    snprintf(g_szWizardNameList[0],
             sizeof(g_szWizardNameList[0]),
             "%s", "app1");

    AppRunWizard(&w, &o);

    return 0;
}
