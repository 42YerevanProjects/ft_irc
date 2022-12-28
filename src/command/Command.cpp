/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:00:15 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 19:15:17 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Command::Command(Server* srv, bool auth) :_srv(srv), _auth(auth) {}

Command::~Command() {}

bool    Command::auth_required() const { return _auth; }
