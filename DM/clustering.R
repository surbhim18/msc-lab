#Generating random numbers
num <- sample(50:100, 5, replace=TRUE)
num

#Array indices begin at 1
d1 <- rnorm(num[1], mean=100, sd=15)

#d2 <- rnorm(num[2], mean=100, sd=15)
#d3 <- rnorm(num[3], mean=100, sd=15)
#d4 <- runif()
#d5 <- rexp()

#plot(density(d1), main = "Density")
#hist(x, probability=FALSE)
#rnorm
#runif
#rexp
#mvrnorm(n = 1, mu, Sigma, tol = 1e-6, empirical = FALSE, EISPACK = FALSE)


