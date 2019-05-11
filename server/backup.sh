world_name="Agora"

mkdir -p `dirname "$0"`/backup
tar cfzv `dirname "$0"`/backup/`date +%F-%H-%M-%S`.tar.gz "$world_name" "$world_name"_nether "$world_name"_the_end
		