{% set replicas_list = [] -%}
{% if mariadb_replicas_group is defined and mariadb_replicas_group %}
{%-   for node in groups[mariadb_replicas_group] %}
  {%-   set _ = replicas_list.append(hostvars[node]['ansible_' + mariadb_bind_interface].ipv4.address) -%}
{%   endfor -%}
{% endif %}
{# #}
{% set mysql_allow = [] -%}
{%- for elt in mariadb_filter_allow_mariadb_port %}
  {% if elt in hostvars %}
  {# this is a node #}
    {%- set _ = mysql_allow.append(hostvars[elt]['ansible_' + mariadb_replicas_interface].ipv4.address) -%}
  {% elif elt|ipaddr %}
  {# this is an IP #}
    {%- set _ = mysql_allow.append(elt) -%}
  {% else %}
  {# this is a group #}
    {% for hst in groups[elt] %}
      {%- set _ = mysql_allow.append(hostvars[hst]['ansible_' + mariadb_replicas_interface].ipv4.address) -%}
    {% endfor %}
  {% endif %}
{% endfor -%}
