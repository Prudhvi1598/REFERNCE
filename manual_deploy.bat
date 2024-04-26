.\install.exe -lock_db infodba infodba dba
.\business_model_extractor.exe -u=infodba -p=infodba -g=dba -mode=all -outfile=D:\TC_13\TC_DATA\model\model_backup.xml
call .\bmide_consolidator.bat -dir=D:\TC_13\TC_DATA\model -file=D:\TC_13\TC_DATA\model\model.xml -consolidate=all
call .\bmide_comparator.bat -compare=all -old=D:\TC_13\TC_DATA\model\model_backup.xml -new=D:\TC_13\TC_DATA\model\model.xml -delta=D:\TC_13\TC_DATA\model\delta.xml
.\clearlocks.exe -assert_all_dead -u=infodba -p=infodba -g=dba
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=schema -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=non_schema -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=bmf_rules -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=lovs -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=types -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=meta -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=rules -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file=D:\TC_13\TC_DATA\model\delta.xml -update=lov_attaches -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file="D:\TC_13\TC_DATA\model\lang\delta_lang.xml" -update=localization -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file="D:\TC_13\TC_DATA\model\delta.xml" -update=all -process=all -mode=upgrade
.\business_model_updater.exe -u=infodba -p=infodba -g=dba -file="D:\TC_13\TC_DATA\model\delta.xml" -update=all -process=delete -mode=upgrade
.\install -regen_schema_file -u=infodba -p=infodba -g=dba
.\install.exe -unlock_db infodba infodba dba
.\install.exe -gen_xmit_file infodba infodba dba
call .\bmide_generatetcplmxmlschema.bat -u=infodba -p=infodba -g=dba
call .\bmide_setupknowledgebase.bat -u=infodba -p=infodba -g=dba -regen=true
.\generate_metadata_cache.exe -u=infodba -p=infodba -g=dba -delete
.\generate_metadata_cache.exe -u=infodba -p=infodba -g=dba -force
.\generate_client_meta_cache.exe -u=infodba -p=infodba -g=dba delete all
.\generate_client_meta_cache.exe -u=infodba -p=infodba -g=dba generate -l1=en_US
