# Copyright 2018-2024 Nick Brassel (@tzarc)
# SPDX-License-Identifier: GPL-2.0-or-later

# avoid 'Entering|Leaving directory' messages
MAKEFLAGS += --no-print-directory

QMK_USERSPACE := $(patsubst %/,%,$(dir $(shell realpath "$(lastword $(MAKEFILE_LIST))")))
ifeq ($(QMK_USERSPACE),)
    QMK_USERSPACE := $(shell pwd)
endif

# Add extra binaries to path
#export PATH := $(QMK_USERSPACE)/bin:$(PATH)

# Deal with macOS
ifeq ($(shell uname -s),Darwin)
SED = gsed
ECHO = gecho
else
SED = sed
ECHO = echo
endif

#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Cleaning

.PHONY: clean distclean userspace_clean

userspace_clean:
	@rm -f $(QMK_USERSPACE)/compile_commands.json || true

clean: userspace_clean
	@$(MAKE) -C $(QMK_USERSPACE)/qmk_firmware clean

distclean: userspace_clean
	@$(MAKE) -C $(QMK_USERSPACE)/qmk_firmware distclean

#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Builds

# short name ! keyboard path ! keymap name
BOARD_DEFS := \
	reverb!dasky/reverb!dasky \
	crkbd!crkbd!dasky \

# short name ! keyboard path ! keymap name ! link source ! link target
LINKED_BOARD_DEFS := \
	nah!keyboards/dasky/nah!dasky!keyboards-dev/nah \
	echo!keyboards/dasky/echo!dasky!keyboards-dev/echo \
	stinky!keyboards/stinky!default!keyboards-dev/stinky \
	prime!keyboards/steelseries/prime_plus!default!mouse-dev/prime_plus \
	g303!keyboards/logitech/g303!default!mouse-dev/g303 \
	cp!keyboards/mb/cp!dasky!keyboards-dev/cp \
	sp!keyboards/dasky/sp!default!keyboards-dev/sp \

keyboards_folder := keyboards/
empty :=
define handle_board_entry
board_name_$1 := $$(word 1,$$(subst !, ,$1))
board_keyboard_$1 := $$(word 2,$$(subst !, ,$1))
board_keyboard_stripped_$1 := $$(subst $$(keyboards_folder),$$(empty),$$(board_keyboard_$1))
board_keymap_$1 := $$(word 3,$$(subst !, ,$1))

.PHONY: $$(board_name_$1) bin_$$(board_name_$1)

$$(board_name_$1): bin_$$(board_name_$1)

bin_$$(board_name_$1):
	@$(ECHO) -e "\e[38;5;14mBuilding: $$(board_keyboard_stripped_$1):$$(board_keymap_$1)\e[0m"
	+@cd "$(QMK_USERSPACE)/qmk_firmware" \
		&& { \
			[ -z "$$(NO_COMPILEDB)" ] \
				&& qmk compile --compiledb -kb $$(board_keyboard_stripped_$1) -km $$(board_keymap_$1) \
				|| qmk compile -kb $$(board_keyboard_stripped_$1) -km $$(board_keymap_$1); \
		}
	@[ -e "$$(QMK_USERSPACE)/qmk_firmware/compile_commands.json" ] && cp $$(QMK_USERSPACE)/qmk_firmware/compile_commands.json $$(QMK_USERSPACE) \
		|| true
endef
$(foreach board_entry,$(BOARD_DEFS),$(eval $(call handle_board_entry,$(board_entry))))

define handle_linked_board_entry
board_name_$1 := $$(word 1,$$(subst !, ,$1))
board_keyboard_$1 := $$(word 2,$$(subst !, ,$1))
board_keymap_$1 := $$(word 3,$$(subst !, ,$1))
link_source_$1 := $$(word 4,$$(subst !, ,$1))
link_target_$1 := $$(word 5,$$(subst !, ,$1))

ifeq ($$(link_target_$1),)
link_target_$1 := $$(board_keyboard_$1)
endif

bin_$$(board_name_$1): link_$$(link_target_$1)

.PHONY: links link_$$(link_target_$1) unlinks unlink_$$(link_target_$1)

links: link_$$(link_target_$1)
link_$$(link_target_$1):
	@if [ ! -L "$(QMK_USERSPACE)/qmk_firmware/$$(link_target_$1)" ] ; then \
		$(ECHO) -e "\e[38;5;14mSymlinking: $$(link_source_$1) -> $$(link_target_$1)\e[0m" ; \
		ln -sf $(QMK_USERSPACE)/$$(link_source_$1) $(QMK_USERSPACE)/qmk_firmware/$$(link_target_$1) ; \
		if [ -z "$$(grep -P '^$$(link_target_$1)$$$$' $(QMK_USERSPACE)/qmk_firmware/.git/info/exclude 2>/dev/null)" ] ; then \
			$(ECHO) $$(link_target_$1) >> $(QMK_USERSPACE)/qmk_firmware/.git/info/exclude ; \
		fi ; \
	fi

unlinks: unlink_$$(link_target_$1)
unlink_$$(link_target_$1):
	@if [ -L "$(QMK_USERSPACE)/qmk_firmware/$$(link_target_$1)" ] ; then \
		$(ECHO) -e "\e[38;5;14mRemoving symlink: $$(link_source_$1) -> $$(link_target_$1)\e[0m" ; \
		rm $(QMK_USERSPACE)/qmk_firmware/$$(link_target_$1) || true; \
		$(SED) -i "\@^$$(link_target_$1)@d" $(QMK_USERSPACE)/qmk_firmware/.git/info/exclude; \
	fi

clean: unlink_$$(link_target_$1)
distclean: unlink_$$(link_target_$1)

generated-files: links
endef
$(foreach board_entry,$(LINKED_BOARD_DEFS),$(eval $(call handle_board_entry,$(board_entry))))
$(foreach board_entry,$(LINKED_BOARD_DEFS),$(eval $(call handle_linked_board_entry,$(board_entry))))

.PHONY: all
all: nah echo stinky prime g303 reverb crkbd sp cp

