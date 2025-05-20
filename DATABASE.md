# Database guide

## Setup
- Install [Docker Desktop](https://www.docker.com/products/docker-desktop/)
- Create a `mariadb:latest` container
- Open command line
- Go to `.\sql`
- Run `docker cp cropiq_dev.sql container_name:/cropiq.sql`
- Run `docker exec -it container_name sh`
- Write `mariadb -u {user} -p`
- Enter your password
- Create a `cropiq` database `CREATE DATABASE cropiq;`
- `USE cropiq;`
- `SOURCE ./cropiq.sql`
