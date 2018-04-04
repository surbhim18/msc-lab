## write all points in files when done. create another script that just uses them.

#Generating random numbers
num <- sample(50:100, 5, replace=TRUE)

library(MASS)
#First distribution - Gaussian
u1 <- c(-7,0)
sigma1 <- matrix(c(10,2,2,2),2,2)
d1 <- mvrnorm(n = num[1], u1 , sigma1)
#plot(d1, xlab = "X", ylab = "Y",col = "dark blue", main = "Normal")

#Second distribution - Gaussian
u2 <- c(12,12)
sigma2 <- matrix(c(2,3,3,10),2,2)
d2 <- mvrnorm(n = num[2], u2 , sigma2)
#plot(d2, xlab = "X", ylab = "Y",col = "red", main = "Normal")

#Third distribution - Gaussian
u3 <- c(5,10)
sigma3 <- matrix(c(10,4,5,20),2,2)
d3 <- mvrnorm(n = num[3], u3 , sigma3)
#plot(d3, xlab = "X", ylab = "Y",col = "red", main = "Normal")

#library downloaded from https://cran.r-project.org/web/packages/LaplacesDemon/index.html
library(LaplacesDemon)
#Fourth distribution - Exponential
u4 <- c(-10,15)
sigma4 <- matrix(c(10,5,5,20),2,2)
d4 <- rmvpe(num[4], u4, sigma4, kappa=1)

#Fifth distribution - Uniform
xmin <- min(d1[,1],d2[,1],d3[,1],d4[,1])
xmax <- max(d1[,1],d2[,1],d3[,1],d4[,1])
ymin <- min(d1[,2],d2[,2],d3[,2],d4[,2])
ymax <- max(d1[,2],d2[,2],d3[,2],d4[,2])
d5 <- matrix(c(runif(num[5],xmin,xmax),runif(num[5],ymin,ymax)),num[5],2)


#find min y from all plots, max y from all plots
xlim = c(xmin,xmax)
ylim = c(ymin,ymax)

#plotting all points in one graph with different colors
color <- c("blue","red","green","yellow","black")
plotchar <- seq(1,6)

plot(d1, type="p", col=color[1], pch=plotchar[1], xlim = xlim, ylim=ylim)
points(d2, col=color[2], pch=plotchar[2])
points(d3, col=color[3], pch=plotchar[3])
points(d4, col=color[4], pch=plotchar[4])
points(d5,col=color[5],pch=plotchar[5])

legend(xlim[2]-6, ylim[2]-1, legend=c("dist 1","dist 2","dist 3","dist 4","dist 5") , cex=0.8, col=color, pch=plotchar, title="Data points")

#assign class labels to points 
#combine points
#each label should be one color


#d4 <- runif()
#d5 <- rexp()

#plot(density(d1), main = "Density")
#hist(x, probability=FALSE)
#rnorm
#runif
#rexp
#mvrnorm(n = 1, mu, Sigma, tol = 1e-6, empirical = FALSE, EISPACK = FALSE)


