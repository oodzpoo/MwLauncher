# MwLauncher
MW Launcher

# Install MariaDB Connector c/c++

* Referance URL : https://mariadb.com/kb/en/library/yum/

* Make MariaDB Repository File
```
sudo vi /etc/yum.repos.d/MariaDB.repo 
```

```
[mariadb]
name = MariaDB
baseurl = http://yum.mariadb.org/10.3/centos7-amd64
gpgkey=https://yum.mariadb.org/RPM-GPG-KEY-MariaDB
gpgcheck=1
```

* Dowonload cache
```
curl -sS https://downloads.mariadb.com/MariaDB/mariadb_repo_setup | sudo bash
```

* Search MariaDB in repo'
```
yum search MariaDB
```

* Install connector
```
sudo yum -y install MariaDB-devel.x86_64 MariaDB-shared.x86_64
```


