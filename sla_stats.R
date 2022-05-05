library(reshape2)
library(ggplot2)
library(scales)

SA <- read.table("data/SA_random.data",
                 header = T,
                 row.names = 1)
MA <- read.table("data/MA.data", header = T, row.names = 1)
best <-
  read.table("mkp_instances/best_known_values.txt",
             header = T,
             row.names = 1)

relative_percentage_deviation <- function(value) {
  relative <- 100 * (best$best - value) / best$best
  
  return (relative)
  
}

SA_rel <- relative_percentage_deviation(SA)
MA_rel <- relative_percentage_deviation(MA)

print(mean(SA_rel[1:30, ]))
print(mean(SA_rel[31:60, ]))
print(mean(MA_rel[1:30, ]))
print(mean(MA_rel[31:60, ]))

ggplot(NULL) +
  geom_point(aes(x = SA_rel[1:30,], y = MA_rel[1:30,], col = "Size 100")) +
  geom_point(aes(x = SA_rel[31:60,], y = MA_rel[31:60,], col = "Size 250")) +
  theme_linedraw() +
  ylim(-0.06, 0.06) +
  geom_smooth(
    method = 'lm',
    formula = y ~ x,
    aes(x = SA_rel[1:30,], y = MA_rel[1:30,], col = "Size 100"),
    fullrange = TRUE,
    alpha = 0.1
    ) +
  geom_smooth(
    method = 'lm',
    formula = y ~ x,
    aes(x = SA_rel[31:60,], y = MA_rel[31:60,], col = "Size 250"),
    fullrange = TRUE,
    alpha = 0.1
    )

print(wilcox.test(SA_rel[,], MA_rel[,], paired=TRUE))

ggplot(melt(data.frame(SA_rel, MA_rel)), aes(variable, value)) + geom_boxplot() + theme_linedraw() +
  scale_x_discrete(labels=c("Simulated Annealing", "Memetic Algorithm")) +
  labs(x="Algorithm", y="Relative percentage deviation")
