{% set slaves_list = [] -%}
{% if mariadb_slaves_group is defined %}
{%-   for node in groups[mariadb_slaves_group] %}
  {%-   set _ = slaves_list.append(hostvars[node]['ansible_' + mariadb_bind_interface].ipv4.address) -%}
{%   endfor -%}
{% endif %}
{# #}
{% set mysql_allow = [] -%}
{%- for elt in mariadb_filter_allow_mariadb_port %}
  {% if elt in hostvars %}
  {# this is a node #}
    {%- set _ = mysql_allow.append(hostvars[elt]['ansible_' + mariadb_slaves_interface].ipv4.address) -%}
  {% elif elt|ipaddr %}
  {# this is an IP #}
    {%- set _ = mysql_allow.append(elt) -%}
  {% else %}
  {# this is a group #}
    {% for hst in groups[elt] %}
      {%- set _ = mysql_allow.append(hostvars[hst]['ansible_' + mariadb_slaves_interface].ipv4.address) -%}
    {% endfor %}
  {% endif %}
{% endfor -%}
