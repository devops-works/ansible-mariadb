# mariadb server role

mariadb database server role

This role supports can install standalone server or replication topologies.

It comes with batteries included:

- Hourly backup integration with ansible-backupninja role (https://github.com/devops-works/ansible-backupninja)
- (optional) filtering support (via [leucos.ferm](https://github.com/leucos/ansible-ferm))

## Requirements

MySQLdb python package (required by `mysql_*` Ansible modules)

## Role Variables

### Backup

Define `mariadb_backup_*` variables if you want to create database backups.
It will install a backupscript in `/etc/backup.d/hourly`.

- `mariadb_backup` (default: false): whether to run database backups
- `mariadb_backup_exclude` (default: ""): `grep` exclude pattern for databases
- `mariadb_backup_keep` (default: 30): how many backups do we keep locally
- `mariadb_backup_s3bucket` (default: ""): S3 bucket to export backups to
- `mariadb_backup_gcloudbucket` (default: ""): GCS bucket to export backups to
- `mariadb_backup_destination` (default: "/var/backups/mysql/"): path to store
  local backups in
- `mariadb_backup_cron_time` (default: "15 */2 * * 0-7"): cron entry for
  backups
- `mariadb_backup_gpg_keys_urls` (default: []): list of gpg keys URLS to
  encrypt backups for

### MySQL config related variables

#### Special vars

The required version is set in `mariadb_version`. The default is "5.6"; The only
other supported version is "5.5".

If defined, `mariadb_bind_interface` takes precedence over
`mariadb_bind_address`. Note that `mariadb_bind_interface` *must* be defined for
master replica setup to work.

#### my.cnf vars

All variables below are their MySQL equivalent.

- `mariadb_bind_address` (default: "127.0.0.1")
- `mariadb_character_set_server` (default: none)
- `mariadb_collation_server` (default: none)
- `mariadb_key_buffer` (default: "16M"; if set to "auto", will be set to 20% of
  total RAM)
- `mariadb_master_host` (default: undefined)
- `mariadb_max_binlog_size` (default: 100M)
- `mariadb_max_connections` (default: 151)
- `mariadb_max_heap_table_size` (default: 16777216)
- `mariadb_open_files_limit` (default: 2565)
- `mariadb_port` (default: 3306)
- `mariadb_query_cache_limit` (default: 1M)
- `mariadb_query_cache_size` (default: 16M)
- `mariadb_query_cache_strip_comments` (default: 0)
- `mariadb_query_cache_type` (default: 0)
- `mariadb_server_id` (default: none)
- `mariadb_sql_mode` (default: none)
- `mariadb_thread_cache_size` (default: 8)
- `mariadb_tmp_dir` (default: /tmp)
- `mariadb_tmp_table_size` (default: 16777216)

#### Custom vars

If you want to use variables that are not supported by the template, you can
fill `mariadb_custom` with whatever variable is needed under it's section.

For instance:

```yaml
mariadb_custom:
  mysqld:
    slave_skip_errors: 1062
  mysql:
    default-character-set: utf8mb4
```

would configure mysqld to skip error "1062" when replicating, and also set the
default character set for mysql clients.

### Users

`mariadb_users` contains a userlist like so:

```yaml
  mariadb_users:
    - name: foo,
      password: "bar",
      priv: "*.*:ALL"
      host: "somehost"
```

A replication user can be setup with `mariadb_replication_user` and
`mariadb_replication_password` (default for both is false, which means no
replication user).

Root's password can be set in `mariadb_root_password` (default: none,
*mandatory*)

### Replication

If replicas can act as masters for other replicas, `mariadb_replicas_as_masters`
should be set to true (default: false). 

Also, for replication to be set-up, `mariadb_replicas_group` should point to an
inventory group (default: false).

### Firewalling

If you want to deploy ferm rules, `mariadb_ferm_enabled` should be set to true
(default: ferm_enabled | default(false)). If you want to deploy nftables rules for the [ansible-nftables](https://github.com/devops-works/ansible-nftables) role, `mariadb_nftables_enabled` should be set to true
(default: false). Variable
`mariadb_filter_allow_mariadb_port` is a list that accepts inventory host
names, group names or ip ranges. By default, it is empty (`[]`) which means mysql
port will be filtered to all hosts.

In order to set-up proper [ferm](https://galaxy.ansible.com/detail#/role/6120)
rules, replicas network interface must be the same across all replicas, and should
be named in `mariadb_replicas_interface` (default: "{{ mariadb_bind_interface
}}"). If it is not set, the role will use `mariadb_bind_interface`.

### Monitoring support

If both `mariadb_monitoring_user` and `mariadb_monitoring_password` are
defined, a mysql user will be created for monitoring.

- `mariadb_monitoring_user`: MySQL user for monitoring integration (default: `false`)
- `mariadb_monitoring_password`: MySQL user for monitoring integration (default: `false`)

Usage
-----

The role is supposed to be used this way from a playbook ("www", "dbreplicas"
and "dbmaster" are some groups/hosts defined in Ansible inventory):

```yaml
- hosts: database
  roles:
    - role: leucos.mariadb
      mariadb_filter_allow_mariadb_port: [ "www" ]
      mariadb_replicas_group: dbreplicas
      mariadb_master_host: dbmaster
      mariadb_replication_user: replicator
      mariadb_replication_password: 0mgpass
      mariadb_root_password: fafdda28e3f
```

Of course, all the variables could be in a group_vars file (best practice, but
it is shorter to present it this way, and it can be used to create various
replication topologies).

Example master host vars:

```yaml
mariadb_server_id: 1
```

Example replica host vars:

```yaml
mariadb_backup:
  keep: 360
  s3bucket: my-awesome-bucker
  destination: /var/backups/mysql/
  cron_time: "15 * * * 0-7"

mariadb_bind_interface: em2
mariadb_server_id: 2
```

# Dependencies

None

# Warning

Try this role many times and ensure it fits your needs before using it for
production...

While this role can help setting up master-replica replication or NEW servers,
it won't help you setup replication for already deployed servers.

# License

MIT

# Author Information

[@devops-works](https://github.com/devops-works)

Patches welcome!
