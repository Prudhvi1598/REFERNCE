#include <ae/ae.h>
#include <tccore/aom.h>
#include <tc/emh.h>
#include <ict/ict_userservice.h>
#include <tc/tc.h>
#include <tccore/tctype.h>
#include <sa/tcfile.h>
#include <tccore/grm.h>
#include <tccore/item.h>
#include <ss/ss_errors.h>
#include <stdarg.h>

static void ECHO(char *format, ...)
{
    char msg[1000];
    va_list args;
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);
    printf(msg);
    TC_write_syslog(msg);
}

#define IFERR_ABORT(X)  (report_error( __FILE__, __LINE__, #X, X, TRUE))
#define IFERR_REPORT(X) (report_error( __FILE__, __LINE__, #X, X, FALSE))

static int report_error(char *file, int line, char *call, int status,
    logical exit_on_error)
{
    if (status != ITK_ok)
    {
        int
            n_errors = 0,
            *severities = NULL,
            *statuses = NULL;
        char
            **messages;

        EMH_ask_errors(&n_errors, &severities, &statuses, &messages);
        if (n_errors > 0)
        {
            ECHO("\n%s\n", messages[n_errors-1]);
            EMH_clear_errors();
        }
        else
        {
            char *error_message_string;
            EMH_get_error_string (NULLTAG, status, &error_message_string);
            ECHO("\n%s\n", error_message_string);
        }

        ECHO("error %d at line %d in %s\n", status, line, file);
        ECHO("%s\n", call);

        if (exit_on_error)
        {
            ECHO("Exiting program!\n");
            exit (status);
        }
    }

    return status;
}

static int ask_file_format(char *path)
{
    IMF_file_status
        stats;

    IFERR_ABORT(IMF_stat(path, &stats));

    return stats.fmt;
}

static void ask_imanfile_path(tag_t fileTag, char *path)
{
    int
        machine_type;
    tag_t
        volume;

    IFERR_ABORT(IMF_ask_volume(fileTag, &volume));
    IFERR_ABORT(VM_ask_machine_type(volume, &machine_type));
    IFERR_ABORT(IMF_ask_file_pathname(fileTag, machine_type, path));
}

static tag_t import_os_file(char *os_path_name, char *new_os_name)
{
    int
        file_type;
    tag_t
        file_tag = NULLTAG;
    char
        pathname[SS_MAXPATHLEN];
    IMF_file_t
        file_descriptor;

    file_type = ask_file_format(os_path_name);

    IFERR_ABORT(IMF_import_file(os_path_name, new_os_name, file_type,
                 &file_tag, &file_descriptor));
    ask_imanfile_path(file_tag, pathname);
    ECHO("imported %s to %s\n", os_path_name, pathname);

    if (strcmp(new_os_name, ""))
        IFERR_REPORT(IMF_set_original_file_name(file_tag, new_os_name));

    IFERR_ABORT(AOM_save(file_tag));
    IFERR_ABORT(AOM_unlock(file_tag));

    return file_tag;
}

static void add_dataset_named_reference(tag_t dataset, char *reference_name,
    tag_t imported_file)
{

    IFERR_ABORT(AOM_refresh(dataset, TRUE));

    IFERR_ABORT(AE_add_dataset_named_ref(dataset, reference_name, AE_PART_OF,
        imported_file));

    IFERR_ABORT(AE_save_myself(dataset));
    IFERR_ABORT(AOM_unlock(dataset));
}

static void do_it(void)
{
    tag_t
        item = NULLTAG,
        rev = NULLTAG,
        dataset = NULLTAG,
        ds_type = NULLTAG,
        relation_type = NULLTAG,
        relation = NULLTAG,
        tool = NULLTAG,
        imported_file = NULLTAG;
    char
        ds_name[WSO_name_size_c+1],
        item_id[ITEM_id_size_c+1],
        new_path[IMF_filename_size_c + 1],
        rev_id[ITEM_id_size_c+1];
    IMF_file_t
        descriptor;

    printf("Enter Item ID: ");
    gets(item_id);
    if (!strcmp(item_id, "")) return;

    printf("Enter %s Rev: ", item_id);
    gets(rev_id);
    if (!strcmp(rev_id, "")) return;

    printf("Enter path to JT file: ");
    gets(new_path);

    ECHO("Searching for Item ID %s with Rev ID %s...\n", item_id, rev_id);
    IFERR_REPORT(ITEM_find_rev(item_id, rev_id, &rev));
    if (rev == NULL_TAG)
    {
        ECHO("ItemRev %s/%s was NOT found!\n", item_id, rev_id);
        return;
    }

    IFERR_ABORT(AE_find_datasettype("DirectModel", &ds_type));
    if (ds_type == NULLTAG)
    {
        ECHO("dataset type DirectModel not found...Exiting Program!\n");
        return;
    }

    sprintf(ds_name, "%s-%s", item_id, rev_id);
    IFERR_ABORT(AE_create_dataset_with_id(ds_type, ds_name,
        "ITK created", ds_name, "A", &dataset));
    IFERR_ABORT(AE_find_tool("PV", &tool));
    if (tool == NULLTAG)
    {
        ECHO("PV Tool not found!\n");
        return;
    }
    IFERR_ABORT(AE_set_dataset_tool(dataset, tool));
    IFERR_ABORT(AE_set_dataset_format(dataset, "BINARY"));
    IFERR_ABORT(AOM_save(dataset));

    IFERR_REPORT(GRM_find_relation_type("IMAN_Rendering", &relation_type));
    if (relation_type == NULLTAG)
    {
        ECHO("IMAN_rendering relation type not found!\n");
        return;
    }
    IFERR_REPORT(GRM_create_relation(rev, dataset, relation_type, NULLTAG,
        &relation));
    IFERR_REPORT(GRM_save_relation(relation));
    IFERR_REPORT(AOM_unload(rev));

    imported_file = import_os_file(new_path, "");
    add_dataset_named_reference(dataset, "JTPART", imported_file);

}

int ITK_user_main(int argc, char* argv[])
{
    IFERR_REPORT(ITK_initialize_text_services(ITK_BATCH_TEXT_MODE));
    IFERR_ABORT(ITK_auto_login());
    IFERR_REPORT(ITK_set_journalling(TRUE));

    do_it();

    IFERR_REPORT(ITK_exit_module(FALSE));

    return ITK_ok;
}
