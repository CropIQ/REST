# Database guide

## Setup
- Install [Docker Desktop](https://www.docker.com/products/docker-desktop/)
- Create a `mariadb:latest` container
- Go to `.\sql`
- Open cmd
- Run `docker cp cropiq_dev.sql {container_name}:/cropiq.sql`
- Run `docker exec -it CropIQDB sh`
- Write `mariadb -u {user} -p`
- Enter your password
- Create a `cropiq` database
- `USE DATABASE cropiq;`
- `SOURCE ./cropiq.sql`