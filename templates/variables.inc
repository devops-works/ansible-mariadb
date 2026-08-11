{# Replicas addresses, split by family. #}
{% set replicas_list_v4 = [] -%}
{% set replicas_list_v6 = [] -%}
{% if mariadb_replicas_group is defined and mariadb_replicas_group %}
{%-   for node in groups[mariadb_replicas_group] %}
{%-     set iface = hostvars[node]['ansible_facts'][mariadb_bind_interface] -%}
{%-     set _ = replicas_list_v4.append(iface.ipv4.address) -%}
{%-     if iface.ipv6 is defined and iface.ipv6 | length > 0 %}
{%-       set _ = replicas_list_v6.append(iface.ipv6[0].address) -%}
{%-     endif %}
{%   endfor -%}
{% endif %}
{# #}
{# Allowed clients, split by family. Entries can be a node, an IP/CIDR or a group. #}
{% set mysql_allow_v4 = [] -%}
{% set mysql_allow_v6 = [] -%}
{%- for elt in mariadb_filter_allow_mariadb_port %}
  {% if elt in hostvars %}
  {# this is a node #}
    {%- set iface = hostvars[elt]['ansible_facts'][mariadb_replicas_interface] -%}
    {%- set _ = mysql_allow_v4.append(iface.ipv4.address) -%}
    {%- if iface.ipv6 is defined and iface.ipv6 | length > 0 %}
      {%- set _ = mysql_allow_v6.append(iface.ipv6[0].address) -%}
    {%- endif %}
  {% elif elt | ansible.utils.ipv4 %}
  {# this is a v4 IP or network #}
    {%- set _ = mysql_allow_v4.append(elt) -%}
  {% elif elt | ansible.utils.ipv6 %}
  {# this is a v6 IP or network #}
    {%- set _ = mysql_allow_v6.append(elt) -%}
  {% else %}
  {# this is a group #}
    {% for hst in groups[elt] %}
      {%- set iface = hostvars[hst]['ansible_facts'][mariadb_replicas_interface] -%}
      {%- set _ = mysql_allow_v4.append(iface.ipv4.address) -%}
      {%- if iface.ipv6 is defined and iface.ipv6 | length > 0 %}
        {%- set _ = mysql_allow_v6.append(iface.ipv6[0].address) -%}
      {%- endif %}
    {% endfor %}
  {% endif %}
{% endfor -%}
{# #}
{# Mixed lists, kept for the ferm template. #}
{% set replicas_list = replicas_list_v4 + replicas_list_v6 -%}
{% set mysql_allow = mysql_allow_v4 + mysql_allow_v6 -%}
