#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <tc/tc_startup.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <sa/user.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_date.h>
#include <fclasses/tc_time.h>

int ITK_user_main(int argc, char* argv[]) {
	int iFail = ITK_ok;
	tag_t item = NULLTAG;
	int rev_num = 0;
	tag_t* rev_values = NULL;
	date_t released_date;
	tag_t latest_released = NULLTAG;
	char* value = NULL;
	char* date_str = NULL;

	iFail = ITK_init_module("infodba", "infodba", "dba");

	if (iFail == ITK_ok) {
		printf("Login successful\n");
		ITEM_find_item("000057", &item);
		AOM_ask_value_tags(item, "revision_list", &rev_num, &rev_values);

		time_t time = 0;

		for (int i = 0; i < rev_num; i++) {
			AOM_ask_value_date(rev_values[i], "date_released", &released_date);

			// Convert Teamcenter date to a C-style time structure
			struct tm tm1 = { 0 };

			// Convert tm structure to a formatted string
			char time_buffer[20];  // Sufficient to hold the formatted date
			strftime(time_buffer, sizeof(time_buffer), "%d-%b-%Y %H:%M:%S", &tm1);

			// Parse the string back to a time structure (optional)
			// strptime(time_buffer, "%d-%b-%Y %H:%M:%S", &tm1);

			time_t time1;
			if ((time1 = mktime(&tm1)) != -1) {
				if (time1 > time) {
					time = time1;
					latest_released = rev_values[i];
				}
			}
		}

		char* value = NULL;
		AOM_ask_value_string(latest_released, "object_string", &value);
		printf("Latest released revision is: %s\n", value);
		MEM_free(value);
	}
	else {
		printf("Not successful login\n");
	}

	return iFail;
}
