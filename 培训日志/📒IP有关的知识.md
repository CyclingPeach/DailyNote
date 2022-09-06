[toc]

||地址|账号密码|说明|
|:-|:-|:-:|:-:|
|`访问Wiki`|`http://192.168.151.151:8090/`|`guest/guest`|`不需要输入账号密码也可以直接看`|
|`访问OpenGrok`|`http://192.168.151.220:8080/source/`||`源代码搜索与对照引擎`|
|`访问JIRA`|`http://192.168.151.220:8060/`|||
|`JumpServer堡垒机`|`内网访问192.168.151.222` `外网访问113.57.107.55:8081`|`[LDAP]/密码`|`密码以修改为自己的`|

- `访问FTP`
```bash
>ftp
> open 192.168.151.230
账号: fpt
密码: Tsftp@2022
```

- `默认[GERRIT_SERVER_IP]`
```shell
192.168.151.220     ？（Wiki上是这样写的）
192.168.122.221     ？（实际操作repo init那里是这个）
```

- `SSH Key配置`

```shell
# 需要分别在
'http://192.168.151.220:8090/settings/#/SSHKeys'    # 武汉?

'http://192.168.122.221:8080/login/'                # 西安?
# 登录之后在地址栏后加`settings`|
```


- `gitconfig配置`

```bash
所有使用西安工作站或从西安ip拉的代码，需要额外配置 gitconfig

#filename:~/.gitconfig
[user]
    email = [LDAP]@thundersoft.com
    name = [LDAP]
[color]
    ui = auto
[url "ssh://[LDAP]@192.168.151.220:29418/"]
    pushinsteadOf=ssh://[LDAP]@192.168.122.221:29418/
```

- `repo init操作`

```shell
192.168.
$ repo init -u "ssh://[LDAP]@[GERRIT_SERVER_IP]:29418/edu_roo/platform/manifest" -b edu_roo_202203 -m edu_roo_202203.xml --no-repo-verify
$ repo init -u "ssh://[LDAP]@192.168.122.221:29418/edu_roo/platform/manifest" -b edu_roo_202203 -m edu_roo_202203.xml --no-repo-verify
```


- `查看push提交`
```shell
http://192.168.151.220:8090/q/status:open
```

- `一些账户密码`
```
Gerrit / JIRA / Jumpserver(堡垒机)
账号:[LDAP]     2205_taozhiyuan
密码:ts@123

FTP
账号: ftp
密码: Tsftp@2022
```